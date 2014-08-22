<?php
class MyRecursiveFilterIterator extends RecursiveFilterIterator {
  public function accept() {
    $name = $this->current ()->getFilename ();
    
    if ($name === '..') {
      return false;
    }
    if ($this->isDir ()) {
      // Only **recurse** into subdirectories.
      return TRUE;
    }
    
    return FALSE;
  }
}


class Page {
  private $pageId = 0;
  private $url = "";
  private $layoutFile = "";
  private $cssFiles = array ();
  private $metaCharset = "UTF-8";
  private $title = "Generic page";

  private $body = "";

  public function __construct($pageId) {
    $this->pageId = $pageId;
    $this->setupPage();
  }


  public function addCSS($cssFile) {
//    array_push ( $this->cssFiles, $cssFile );
  }


  public function setTitle($title) {
    $this->title = $title;
  }


  public function replaceMarkers($markers) {
    foreach ( $markers as $key => $val ) {
      $this->body = str_replace ( $key, $val, $this->body );
    }
  }


  public function resolveLocalization() {
    // TODO: read from resource file
    $en = array (
        'USERNAME_LBL' => 'Username',
        'PASSWD_LBL' => 'Password',
        'LOGIN_LBL' => 'Log in' 
    );
    
    $matches = array ();
    $count = preg_match_all ( "/#res:(.*)#/", $this->body, $matches );
    foreach ( $matches [1] as $strId ) {
      $localized = $en [$strId];
      $this->body = str_replace ( "#res:" . $strId . "#", $localized, $this->body );
    }
  }
  

  public function resolveFragments() {
    do {
      $matches = array ();
      $count = preg_match_all ( "/#HTMLF:(.*)#/", $this->body, $matches );
      foreach ( $matches [1] as $fragment ) {
        
        $filePath = $this->findFile($fragment);
        $fragFile = file_get_contents ( $filePath );
        $this->body = str_replace ( "#HTMLF:" . $fragment . "#", $fragFile, $this->body );
      }
    } while ( $count != null && $count > 0 );
  }
  

  /**
   * Find all expressions located in the current page.
   *
   * @return An array with all expressions from the current page. 
   */
  private function getExpressions() {
    $matches = array ();
    $count = preg_match_all ( "/#{(.*)}/", $this->body, $matches );
    return $matches[1];
  }


  private function evaluateExpression($expression) {
    $accessors = explode('.', $expression);

    if ($accessors[0] === 'session') {
      $map = Session::current()->getSessionMap();

      $result = $map[ $accessors[1] ];
    } else if ($accessors[0] === 'data') {
      $map = Session::current()->getRequestMap();
      $map = $map['data'];

      $result = $map[ $accessors[1] ];
    }

    return $result;
  }


  private function resolveExpressions() {
    // call a page specific PHP script, if it exists, to fill additional 
    // data into the request map 
    $pageDir  = implode('/', $this->pagePath);
    $scriptFile = $pageDir . "/page.php";
    if (file_exists($scriptFile)) {
      include $scriptFile;
      getRenderValues();
    }
    
    // get all expressions in the html page
    $expressions = $this->getExpressions();

    // replace all expressions with their corresponding values
    foreach ($expressions as $expression) {
      // Evaluate the expression.
      $value = $this->evaluateExpression($expression);

      // replace the evaluated value in the page.
      $this->body = str_replace("#{" . $expression . "}", $value, $this->body);
    }
  }


  public function sendResponse() {
    $resultHtml = "<!DOCTYPE html>\n" . "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n" . "  <head>\n" . "    <meta charset=\"" . $this->metaCharset . "\">\n";

    $pageDir  = implode('/', $this->pagePath);
    $cssFile = "cache/" . $pageDir . "/" . "page.css";
    $resultHtml = $resultHtml . "    <link rel=\"stylesheet\" type=\"text/css\" href=\"" . $cssFile . "\">\n";
    $resultHtml = $resultHtml . "    <title>" . $this->title . "</title>\n" . "  </head>\n" . "\n" . "  <body>\n" . $this->body . "</body></html>";
    
    echo $resultHtml;
  }


  public function render($markers) {
    $this->body = file_get_contents ( $this->layoutFile);
    $this->resolveFragments ();
    $this->replaceMarkers ( $markers );

    $this->resolveExpressions();
    
    $this->resolveLocalization ();
    $this->sendResponse ();
  }

  // method called from constructor - set up the path to the
  // page, given its page id
  private function setupPage() {
    $this->pagePath = array (
        '0' => '0' 
    );
    if ($this->pageId != 0) {

      $directory = new RecursiveDirectoryIterator ( '0' );
      $filter = new MyRecursiveFilterIterator ( $directory );
      $iterator = new RecursiveIteratorIterator ( $filter );
      foreach ( $iterator as $fileinfo ) {
        $pathName = substr ( $fileinfo->getPathName (), 0, -2);
        $pagePath = explode ( '/', $pathName );
        if (end ( $pagePath ) == $this->pageId) {
          $this->pagePath = $pagePath;
          break;
        }
      }
      
      $this->pageId = end ( $this->pagePath );
    }

    // look up the layout file
    $this->layoutFile = $this->findFile("Layout.html");

    // collect all CSS files up to the top level
    // files lower in the level override files with the same name from upper levels
    // multiple files in the same directory are ordered by the collation order of their file name
    // all files are combined in a single css file which is then referenced in the response
    $this->collectCSS();
  }


  private function collectCSS() {
    // create combined css file for this page
    $pageDir  = implode('/', $this->pagePath);
    $cacheDir = "cache/" . $pageDir;

    // create the cache directory for this page - ignore if dir already exists
    @mkdir($cacheDir, 0755, true);

    $combinedCSS = $cacheDir .  "/" . "page.css";
    file_put_contents($combinedCSS, "");

    $files = $this->collectFiles("/.*\.css/");
    foreach($files as $cssFile) {
      $css = file_get_contents($cssFile);
      file_put_contents($combinedCSS, "/* " . $cssFile . " ********************************************/\n", FILE_APPEND);
      file_put_contents($combinedCSS, $css, FILE_APPEND);
    }
  }
  
  
  private function getSpeakingUrl() {
  }


  /**
   * Traverses the page path up to the root to find a "most specific" file
   *
   * @param string $filename
   *          The name of the file to find.
   *          
   * @return The complete path name of the file
   */
  private function findFile($filename) {
    for($max = count($this->pagePath);  $max > 0;  $max--) {
      $curPath = array_slice($this->pagePath, 0, $max);
      $pathName = implode('/', $curPath);
      $filePath = $pathName . "/" . $filename;

      if (file_exists($filePath)) {
        break;
      }
    }

    return $filePath;
  }

  
  /**
   * Collects all files which match a given pattern from the root page down to
   * the current page.
   * Files lower in the level override files with the same name from upper levels.
   * Multiple files in the same directory are ordered by the collation order of their file name
   */
  private function collectFiles($pattern) {

    $result = array();
    
    // traverse all page directories from top to bottom
    // 0
    // 0/1
    // 0/1/5
    for($level = 1;  $level <= count($this->pagePath);  $level++) {
      $curPath = array_slice($this->pagePath, 0, $level);
      $pathName = implode('/', $curPath);

      $allFiles = new DirectoryIterator($pathName);
      $matchedFiles = new RegexIterator($allFiles, $pattern);

      // get all matching files in the current directory.
      $fileList = array();
      foreach ($matchedFiles as $fileInfo) {
        array_push($fileList, array("key" => $fileInfo->getFilename(),
                                    "path" => $fileInfo->getPathname()));
      }
      asort($fileList);

      // $fileList now contains all matching files in the current directory,
      // sorted by their file name
      
      // add all files from the directory to the result array:
      // * Existing files will be overridden
      // * New files will be appended
      foreach ($fileList as $file) {
        $result[$file['key']] = $file['path'];
      }
    }

    return $result;
  }
  

  /**
   * Reads the page definition file.
   */
  private function readPageData() {
  }
}

?>
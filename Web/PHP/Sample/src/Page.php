<?php

class Page {

	private $body = "";
	private $cssFiles = array();
	private $metaCharset = "UTF-8";

	public function __construct($title) {
	}


	public function addCSS($cssFile) {
		array_push($this->cssFiles, $cssFile);
	}

	public function addFile($htmlFile) {
		$this->body = $this->body . file_get_contents($htmlFile);
	}


	public function replaceMarkers($markers) {
		foreach($markers as $key => $val) {
			$this->body = str_replace($key, $val, $this->body);
		}
	}

	public function resolveFragments() {
		$matches = array();
		
		do {
			$count = preg_match_all("/#HTMLF:(.*)#/", $this->body, $matches);
			foreach ($matches[1] as $fragment) {
				$fragFile = file_get_contents("content/" . $fragment);
				$this->body = str_replace("#HTMLF:".$fragment."#", $fragFile, $this->body);
			}
		}while($count <> null && $count > 0);
	}

	public function sendResponse() {
		$resultHtml = "<!DOCTYPE html>\n" .
				"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n" .
				"  <head>\n" .
				"    <meta charset=\"".$this->metaCharset."\">\n";

		foreach ($this->cssFiles as $cssFile) {
			$resultHtml = $resultHtml . "    <link rel=\"stylesheet\" type=\"text/css\" href=\"".$cssFile."\">\n";
		}

		$resultHtml = $resultHtml . "    <title>".$title."</title>\n" .
				"  </head>\n" .
				"\n" .
				"  <body>\n" .
				$this->body . 
				"</body></html>";

		echo $resultHtml;
	}
}

?>
<?php

require_once "Session.php";
require_once "Page.php";


class Entry {

  private $session;

  public function doGet() {

    if ($this->session->isAuthenticated()) {
      // Normal page request from authenticated user.
      
      $pageId = $_GET['pageId'];
      if ($pageId == null) {
        $pageId = 0;
      }
      $page = new Page($pageId);

    } else {

      if ($_SERVER['REQUEST_URI'] === '/PHPSample/backend.php') {

        // response: Login page
        $page = new Page(1);

      } else {
        // Normal page request which does not require authentication
        
        $pageId = $_GET['pageId'];
        if ($pageId == null) {
          $pageId = 0;
        }
        $page = new Page($pageId);
      }
    }

    $page->render();
  }


  private function redirect($url, $statusCode = 303) {
    header('Location: ' . $url, true, $statusCode);
    die();
  }


  public function doPost() {
    $page = "index.php";

    if ($this->session->isAuthenticated()) {

      if ($_POST['action'] == 'doLogout') {
        $this->session->logout();
      
        // $page->addFile("Logout.html");
      
        // response: Login page
        $page = "index.php?pageId=1"; // new Page(1);
      }

    } else {
      
      if ($_POST['action'] == 'doLogin') {
        $this->session->login($_POST['username'], $_POST['password']);
        if ($this->session->isAuthenticated()) {

          // response: top level page
          // $page = new Page(0);
          $page = "index.php";        
        } else {

          // response: Login page with error message
          //$page = new Page(1);
          //$map = $this->session->getRequestMap();
          //$map['data']['error'] = 'Login failed - invalid username or password!';
          //$this->session->setRequestMap($map);

          $page = "index.php?pageId=1"; // new Page(1);
        }
      }

    }

    // PRG pattern - each POST sends a REDIRECT followed by a GET request!
    $this->redirect($page);
  }


  public function service() {
    $this->session = Session::getSession();

    // setup the request scope map
    $reqMap = array();
    // add some sample data
    $reqMap['data'] = array();
    $reqMap['data']['lorem'] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam et nisl et justo lacinia porta ac ac mi. Aenean elementum facilisis metus nec aliquet. Aenean ultricies dui sed dolor luctus hendrerit in euismod eros. Pellentesque eu enim dictum, sollicitudin ligula eget, adipiscing justo. In porttitor ligula in consequat suscipit. Nulla nec ultrices elit. Curabitur mollis bibendum nibh non ultricies. Ut vel ante ac justo tincidunt vehicula. Quisque at fringilla arcu. In bibendum magna erat, eu hendrerit nibh semper fermentum. Etiam in eleifend velit.";
    $this->session->setRequestMap($reqMap);

    // process GET or POST request
    if ($_SERVER['REQUEST_METHOD'] == 'POST') {
      $this->doPost();
    } else if ($_SERVER['REQUEST_METHOD'] == 'GET') {
      $this->doGet();
    }

    // request done, clear map
    $reqMap = array();
    $this->session->setRequestMap($reqMap);
  }
}

?>

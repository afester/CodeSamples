<?php

require_once "Session.php";
require_once "Page.php";


// authenticated requests
function Main() {
	$session = Session::getSession();
	$markers = array();

	if (!$session->isAuthenticated()) {
	  //echo "NOT AUTH";

		// check if this is a login request
		if ($_SERVER['REQUEST_METHOD'] == 'POST' && $_POST['action'] == 'doLogin') {
	    //echo "LOGIN REQUEST";

		  $session->login($_POST['username'], $_POST['password']);
			if ($session->isAuthenticated()) {

				// response: top level page
				$page = new Page(0);
				$markers['###CONTENT###'] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam et nisl et justo lacinia porta ac ac mi. Aenean elementum facilisis metus nec aliquet. Aenean ultricies dui sed dolor luctus hendrerit in euismod eros. Pellentesque eu enim dictum, sollicitudin ligula eget, adipiscing justo. In porttitor ligula in consequat suscipit. Nulla nec ultrices elit. Curabitur mollis bibendum nibh non ultricies. Ut vel ante ac justo tincidunt vehicula. Quisque at fringilla arcu. In bibendum magna erat, eu hendrerit nibh semper fermentum. Etiam in eleifend velit.";
				$page->render($markers);

			} else {

				// response: Login page with error message
				$page = new Page(1);
				$markers['###ERROR###'] = 'Login failed - invalid username or password';
				$page->render($markers);
			}

		} else {
  		// response: Login page
			$page = new Page(1);
			$markers['###ERROR###'] = '';
			$page->render($markers);
		}

	} else  {
	  //echo "AUTH";

		if ($_SERVER['REQUEST_METHOD'] == 'POST') {
			if ($_POST['action'] == 'doLogout') {
				$session->logout();
	
				// $page->addFile("Logout.html");

	  		// response: Login page
				$page = new Page(1);
				$markers['###ERROR###'] = '';
				$page->render($markers);
			}

		} else {

			// Normal page request from authenticated user.

			$pageId = $_GET['pageId'];
			if ($pageId == null) {
				$pageId = 0;
			}
      $page = new Page($pageId);
	    $markers['###CONTENT###'] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam et nisl et justo lacinia porta ac ac mi. Aenean elementum facilisis metus nec aliquet. Aenean ultricies dui sed dolor luctus hendrerit in euismod eros. Pellentesque eu enim dictum, sollicitudin ligula eget, adipiscing justo. In porttitor ligula in consequat suscipit. Nulla nec ultrices elit. Curabitur mollis bibendum nibh non ultricies. Ut vel ante ac justo tincidunt vehicula. Quisque at fringilla arcu. In bibendum magna erat, eu hendrerit nibh semper fermentum. Etiam in eleifend velit.";
	    $page->render($markers);
		}
	}
}


// anonymous requests
function MainAnonymous() {
	$session = Session::getSession();
	$markers = array();
  // Normal page request which does not require authentication

	$pageId = $_GET['pageId'];
	if ($pageId == null) {
		$pageId = 0;
	}
  $page = new Page($pageId);
	$markers['###CONTENT###'] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam et nisl et justo lacinia porta ac ac mi. Aenean elementum facilisis metus nec aliquet. Aenean ultricies dui sed dolor luctus hendrerit in euismod eros. Pellentesque eu enim dictum, sollicitudin ligula eget, adipiscing justo. In porttitor ligula in consequat suscipit. Nulla nec ultrices elit. Curabitur mollis bibendum nibh non ultricies. Ut vel ante ac justo tincidunt vehicula. Quisque at fringilla arcu. In bibendum magna erat, eu hendrerit nibh semper fermentum. Etiam in eleifend velit.";
	$page->render($markers);
}

?>

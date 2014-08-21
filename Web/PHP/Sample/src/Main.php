<?php

require_once "Session.php";
require_once "Page.php";


/*
 $dbgStr = "<div style=\"background-color:#f0f0f0\" ><pre>" . "Session id:" . session_id() ."\n";
$dbgStr = $dbgStr . "Username:" . $session->getUserName() . "\n</pre>";

$dbgStr = $dbgStr .
"<pre>" .
"Session id:" . session_id() ."\n" .
"</pre>" .
"<b>_SESSION:</b>" .
"<pre style=\"margin-left: 10px\">" . print_r($_SESSION, true). "</pre>" .
"<b>_SERVER:</b>" .
"<pre style=\"margin-left: 10px\">";
foreach ($_SERVER as $key => $value) {
$dbgStr = $dbgStr . $key . " = " . $value . "\n";
}
$dbgStr = $dbgStr . "</pre>" .
"<b>_GET:</b>" .
"<pre style=\"margin-left: 10px\">" .
print_r($_GET, true) .
"</pre>".
"<b>_POST:</b>".
"<pre style=\"margin-left: 10px\">".
print_r($_POST, true).
"</pre>" .
"</div>";
*/

function renderResponse($pageId) {

	$session = Session::getSession();
	$page = new Page($pageId);

	$markers = array();

	// algorithm:
	// * We need a layout file. This is taken from the page with the given id,
	//   and if there is no layout file the one from the page above is taken (recursively)
	// * We resolve all html fragments inside the layout file - again, the fragments
	//   are searched along the current page first, then recursively upwards

	// response: page with given id (or 0 if no id given)
	$page->addCSS("0/Layout.css");	  // todo: generate single css file to reduce roundtrips
	$page->addCSS("0/Style.css");

	$markers['###CONTENT###'] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam et nisl et justo lacinia porta ac ac mi. Aenean elementum facilisis metus nec aliquet. Aenean ultricies dui sed dolor luctus hendrerit in euismod eros. Pellentesque eu enim dictum, sollicitudin ligula eget, adipiscing justo. In porttitor ligula in consequat suscipit. Nulla nec ultrices elit. Curabitur mollis bibendum nibh non ultricies. Ut vel ante ac justo tincidunt vehicula. Quisque at fringilla arcu. In bibendum magna erat, eu hendrerit nibh semper fermentum. Etiam in eleifend velit.";
	$markers['###USERNAME###'] = $session->getUserName();

	$page->render($markers);
}

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
				renderResponse(0);
			} else {
				// response: Login page with error message
				$page = new Page(1);
				$page->addCSS("0/Layout.css");	  // todo: generate single css file to reduce roundtrips
				$page->addCSS("0/Style.css");
				
				$markers['###ERROR###'] = 'Login failed - invalid username or password';
				$page->render($markers);
			}

		} else {
		  //echo "PAGE REQUEST";

  		// response: Login page
			$page = new Page(1);
			$page->addCSS("0/Layout.css");	  // todo: generate single css file to reduce roundtrips
			$page->addCSS("0/Style.css");

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
				$page->addCSS("0/Layout.css");	  // todo: generate single css file to reduce roundtrips
				$page->addCSS("0/Style.css");
				
				$markers['###ERROR###'] = '';
				$page->render($markers);
			}

		} else {

			// Normal page request from authenticated user.

			$pageId = $_GET['pageId'];
			if ($pageId == null) {
				$pageId = 0;
			}
			renderResponse($pageId);

		}
	}
}

// anonymous requests
function MainAnonymous() {
	// Normal page request from non-authenticated user.
	
	$pageId = $_GET['pageId'];
	if ($pageId == null) {
		$pageId = 0;
	}
	renderResponse($pageId);
}

?>

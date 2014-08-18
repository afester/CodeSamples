<?php
require_once "Session.php";

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



function Main() {
	$session = Session::getSession();
	$page = new Page();
	$markers = array();
	
	if (!$session->isAuthenticated()) {
	
		// check if this is a login request
		if ($_SERVER['REQUEST_METHOD'] == 'POST' && $_POST['action'] == 'doLogin') {
	
			$session->login($_POST['username'], $_POST['password']);
			if ($session->isAuthenticated()) {
				$dbgStr = $dbgStr . "User name: " . $session->getUserName();
				$dbgStr = $dbgStr . "Session ID: " . session_id();
	
				$page->addFile("Sample.html");
			} else {
				$page->addFile("Login.html");
				$markers = array('###ERROR###' => 'Login failed');
				$page->replaceMarkers($markers);
			}
	
		} else {
			$page->addFile("Login.html");
			$markers = array('###ERROR###' => '');
		}
	
	} else  {
	
		if ($_SERVER['REQUEST_METHOD'] == 'POST') {
			if ($_POST['action'] == 'doLogout') {
				$session->logout();
	
				// $page->addFile("Logout.html");
	
				$page->addFile("Login.html");
				$markers = array('###ERROR###' => '');
			}
	
		} else {
			$pageId = $_GET['pageId'];
			if ($pageId == null) {
				$pageId = "Sample.html";
			} else {
				$pageId = $pageId . '.html';
			}
			$page->addFile($pageId);
		}
	}
	
	$page->replaceMarkers($markers);
	$page->sendResponse();
}

?>

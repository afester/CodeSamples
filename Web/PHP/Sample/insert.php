<?php

class Session {
	
	private $authenticated = false;
	// private $userName = 'Hallo';
	private $userName = "Hello";
//	private $xyz = 42;
//	private $roles;

	public static function getSession() {
		// start a new session
		session_start();

		// get or create the session object
		$session = $_SESSION['session'];
		var_dump($session);
	//	echo "YES:" . print_r($session, true) . $session->toString();
		if ($session == null) {
//			echo "YES2:" . print_r($session, true) . $session->toString();
			$session = new Session();
			$_SESSION['session'] = $session;
//			echo "YES";
		}
		return $session;
	}

	public function isAuthenticated() {
		return $this->authenticated;
	}
	
	public function getUserName() {
		return $this->userName;
	}

	public function login($username, $password) {
		echo "123";
		
		if ($username == "andreas" && $password == "helloWorld") {
			echo "124";
				
			echo "AUTH";
			session_regenerate_id(true);
			$this->authenticated = true;
			$this->userName = $username;
		}
	}

	public function logout() {
		echo "LOGOUT";

		$authenticated = false;
		$userName = null;

		$_SESSION = array();
		
		if (ini_get("session.use_cookies")) {
			$params = session_get_cookie_params();
			setcookie(session_name(), '', time() - 42000, $params["path"],
			$params["domain"], $params["secure"], $params["httponly"]
			);
		}
		session_destroy();
	}

	public function toString() {
		var_dump($this->userName);
		return "Seffssion[authenticated:" . $this->authenticated . ", userName:" . $this->userName . "]";
	}
}

echo "12";
$session = Session::getSession();

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


if (!$session->isAuthenticated()) {
	echo "13";

	// check if this is a login request
	if ($_SERVER['REQUEST_METHOD'] == 'POST' && $_POST['action'] == 'doLogin') {
	echo "15";
		$session->login($_POST['username'], $_POST['password']);
		if ($session->isAuthenticated()) {
			echo "163";
			$dbgStr = $dbgStr . "User name: " . $session->getUserName();
			$dbgStr = $dbgStr . "Session ID: " . session_id();
			$page = file_get_contents ("Sample.html");
			$page = str_replace("###DEBUG###", $dbgStr, $page);
		} else {
			echo "164";
			$page = file_get_contents ("Login.html");
			$page = str_replace("###ERROR###", "Login failed", $page);
		}  

	} else {
		echo "16";
		$page = file_get_contents ("Login.html");
	}

} else  {
	echo "14";
echo "1";
	if ($_SERVER['REQUEST_METHOD'] == 'POST') {
		if ($_POST['action'] == 'doLogout') {
			$session->logout();
			$page = file_get_contents ("Logout.html");
		}
	} else {
		$dbgStr = $dbgStr . "Session ID: " . session_id();
		$page = file_get_contents ("Sample.html");
		$page = str_replace("###DEBUG###", $dbgStr, $page);
	}
}

echo $page;
?>

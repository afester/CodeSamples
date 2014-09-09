<?php

class Session {

	private $sessionMap = array();
	private $requestMap = array();


	public function __construct() {
	  $this->sessionMap['authenticated'] = false;
	  $this->sessionMap['userName'] = 'anonymous';
	}


	/**
	 * @return A session object corresponding to the current session.
	 */
	public static function getSession() {
		// start a new session
		session_start();

		// get or create the session object
		$session = $_SESSION['session'];
		if ($session == null) {
			$session = new Session();
			$_SESSION['session'] = $session;
		}
		
		return $session;
	}


	public static function current() {
	  return $_SESSION['session'];
	}


	// TODO: Should probably move all the differently scoped maps into a 
	// "Context" class or similar ... The session object itself should
	// really only be an entry in the "Session" map ...
	public function getApplicationMap() {
	  return $this->applicationMap;
	}


	public function getSessionMap() {
	  return $this->sessionMap;
	}


	public function getRequestMap() {
	  return $this->requestMap;
	}
	
	public function setRequestMap($map) {
	  $this->requestMap = $map;
	  
	}

	
	/**
	 * @return <code>true</code> if this session is authenticated, <code>false</code> otherwise.
	 */
	public function isAuthenticated() {
		return $this->sessionMap['authenticated'];
	}


	public function login($username, $password) {
		
		// TODO: read username and password from database; use encrypted password!
		if ($username == "andreas" && $password == "helloWorld") {
			session_regenerate_id(true);

			$this->sessionMap['authenticated'] = true;
			$this->sessionMap['userName'] = $username;
		}
	}


	/**
	 * Logout from the current session.
	 * After this method has been called, the session is again in "not authenticated" state.
	 */
	public function logout() {
		$this->sessionMap['authenticated'] = false;
		$this->sessionMap['userName'] = "anonymous";

		$_SESSION = array();

		if (ini_get("session.use_cookies")) {
			$params = session_get_cookie_params();
			setcookie(session_name(), '', time() - 42000, $params["path"],
			$params["domain"], $params["secure"], $params["httponly"]
			);
		}
		session_destroy();
	}


	/**
	 * @return A readable string of this session object, including the session object attributes.
	 */
	public function toString() {
		return "Session[" . print_r($this->sessionMap, true) . "]";
	}
}

?>
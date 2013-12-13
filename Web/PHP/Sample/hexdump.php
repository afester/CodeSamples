<!DOCTYPE html>
<html>
   <head>
   </head>
   
   <body>

<?php
function hexDump($contents) {
	$result = array();

	$addr = 0;
	$hexDump = "";
	$asciiDump = "";
	$column = 0;
	
	foreach (str_split($contents) as $c) {
		$hexDump = $hexDump . sprintf("%02X ", ord($c));
		if (ord($c) > 31 && ord($c) < 128) {
			$asciiDump = $asciiDump . $c;
		} else {
			$asciiDump = $asciiDump . ".";
		}
	
		$column++;
		if ( ($column % 16) == 0) {
			$line = sprintf("%08X: %-48s%s", $addr, $hexDump, $asciiDump);
			array_push($result, $line);

			$asciiDump = "";
			$hexDump = "";
			$column = 0;
			$addr += 16;
	
		}
	}
	if ($column > 0) {
		$line = sprintf("%08X: %-48s%s", $addr, $hexDump, $asciiDump);
		array_push($result, $line);
	}

	return $result;
}

function renderDump($lines) {
	echo "<pre>";
	foreach ($lines as $line) {
		echo "$line<br/>";
	}
	echo "</pre>";
}

function logDump($lines) {
	foreach ($lines as $line) {
		file_put_contents('php://stderr', $line);
	}
}

$value = "Hello\r\nSun";
$dump = hexDump($value);
renderDump($dump);

echo "<font color=\"red\">str_replace(\"Sun\", \"Moon\", $value) ==></font>";
$value = str_replace("Sun", "Moon", $value);
$dump = hexDump($value);
renderDump($dump);

define('LF', "\n");
define('CR', "\r");

echo "<font color=\"red\">str_replace(CR, \"\", $value) ==></font>";
$value = str_replace(CR, "", $value);
$dump = hexDump($value);
renderDump($dump);
logDump($dump);

?>
   </body>
   
</html>

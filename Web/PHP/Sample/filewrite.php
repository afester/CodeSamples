<?php
$fp = fopen('/tmp/sample.txt', 'w');
fwrite($fp, 'Hello World');
fclose($fp);
echo "File written.";
?>

<!DOCTYPE html>
<html>
   <head>
   </head>
   
   <body>
<?php
   echo "Script: " . $_SERVER['SCRIPT_NAME'] . "<br/>";
   echo "Request URL: " . $_SERVER['REQUEST_URI'] . "<br/>";
?>

   <h2>List of all HTTP headers:</h2>
   <pre>

<?php 

foreach ($_SERVER as $name => $value) {
   echo $name . "=" . $value . "\n";
}

?>
   </pre>

   </body>

</html>
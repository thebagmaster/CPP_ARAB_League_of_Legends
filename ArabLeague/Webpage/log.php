<?php 
include 'mysql_connect.php';
if($_POST[text])
	$text = $_POST[text];
if($_GET[text])
	$text = $_GET[text];
if($_GET[unban])
{
	$query = "DELETE FROM log WHERE ban='$text'";
	mysql_query($query) or die ("Error in query: $query. " . mysql_error());
	mysql_close($con);
}
else
{
	$query = "INSERT INTO log (ban) VALUES('$text')";
	mysql_query($query) or die ("Error in query: $query. " . mysql_error());
	mysql_close($con);
}

// <head>
// <meta http-equiv="refresh" content="1; URL=/">
// </head>
?>
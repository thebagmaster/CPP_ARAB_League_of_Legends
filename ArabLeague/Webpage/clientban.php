<?php 
include 'mysql_connect.php';
$text = $_GET[text];
$ip = $_SERVER['REMOTE_ADDR'];

$query = "INSERT INTO client (ban, ip) VALUES('$text','$ip')";
$result = mysql_query($query);

$query="SELECT * FROM client ORDER BY ban";
$result = mysql_query($query);
$num=mysql_numrows($result);
$i=0;
$perma=0;
$f2="";
while ($i < $num) {
	$f1=mysql_result($result,$i,"ban");
	if($f1==$f2)
		$perma+=1;
	$f2=$f1;
	$i++;
}
echo $perma;
if($perma >= 2)
{
	$query = "INSERT INTO log (ban) VALUES('$text')";
	$result = mysql_query($query);
}
mysql_close($con);
?>
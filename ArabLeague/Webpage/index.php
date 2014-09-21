<?php
include 'mysql_connect.php';
$query="SELECT * FROM log ORDER BY ban ASC LIMIT 50";
$result=mysql_query($query);
$num=mysql_numrows($result);
mysql_close();
$i=0;
while ($i < $num) {
$f1=mysql_result($result,$i,"ban");
echo $f1." ";
$i++;
}
?>

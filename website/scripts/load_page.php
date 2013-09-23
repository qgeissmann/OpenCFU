<?php

//~ if(!$_POST['page']) die("0");

$page = $_POST['page'];
if ($page == "")
	$page = "home";

$file = '../pages/'.$page.'.html';

if(file_exists($file)){
	echo file_get_contents($file);
}

else {
	echo "There is no such page ($file) ! </br>";
	echo $page;
}
?>

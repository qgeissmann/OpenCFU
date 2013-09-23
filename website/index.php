<?php
header('Content-type: text/html; charset=utf-8');
include_once('scripts/make_content.php');
makeContent($_SERVER['REQUEST_URI']);
?>

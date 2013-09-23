
<?php

include_once('scripts/load_navFuns.php');
function clear(){echo "<div class=\"clear\"></div>";}
function makePageKey($currentPage){	
	$file = basename($currentPage);
	preg_match( '/.php/', $file, $match );
	if($file == 'index.php' || $file == '' || $match[0] == '')
		$key = 'home';
	else
		$key = preg_replace("/\\.[^.\\s]{3,4}$/", "", $file);
	return($key);
}	

function makeContent($currentPage){	
	$PAGES_DIR='./pages/';
	$PORTIONS_DIR='./htmlPortions/';
	
	$key=makePageKey($currentPage);
	echo "<!DOCTYPE html>\n";
	echo "<html>";
	include_once($PORTIONS_DIR.'head.html');
	echo "<body>";
	include_once($PORTIONS_DIR.'banner.html');
	clear();
	makeNav($key,$PAGES_DIR);
	clear();
	echo "<div id=\"page\"><div id=\"pageContent\">";
	include_once($PAGES_DIR.$key.'.html');
	echo "</div>";
	include_once($PORTIONS_DIR.'sidebar.html');
	echo "</div>";		
	clear();
	echo "<div id=\"footer\">Software and website by <a href=\"http://quentelery.users.sourceforge.net\">Quentin Geissmann</a><br>Website hosted by <a href='http://sourceforge.net'>sourceforge</a></div></body></html>";
}
?>

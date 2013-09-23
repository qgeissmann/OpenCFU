<?php 
	function makeMenuEntry($pageName,$title,$currentPage){
		if($pageName ==""){
			if($currentPage== "home")
				echo "<li class=\"current border_shape\"> <a href=\"./\">$title</a></li>";
			else
				echo "<li class=\"border_shape\"> <a href=\"./\">$title</a></li>";
		}
		else if($pageName == $currentPage){
			echo "<li class=\"current border_shape\"> <a href=\"./$pageName.php\">$title</a></li>";
		}
		else{
			echo "<li class=\"border_shape\"> <a href=\"./$pageName.php\">$title</a></li>";
		}
	}
	function makeNav($currentPage){
		echo "<div id=\"navdiv\" class=\"mybox\"><ul id=\"navigation\">";
		makeMenuEntry('','Home',$currentPage);
		makeMenuEntry('tuto','Tutorial',$currentPage);
		makeMenuEntry('manual','Manual',$currentPage);
		makeMenuEntry('samples','Image Samples',$currentPage);
		makeMenuEntry('devcorner','Developers&#8217 corner',$currentPage);
		echo "</ul></div>";
	}
?>



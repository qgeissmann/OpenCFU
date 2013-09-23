for i in $(ls pages);do ln -s -f  ./index.php  $(echo $i | sed s/.html/.php/g) ;done

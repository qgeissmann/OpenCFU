run("Clear Results");
run("Close All");
dir1 = getDirectory("Choose  Directory ");
list = getFileList(dir1);
setBatchMode(false);

a = newArray(list.length+1);
a[0] = getTime();

//~
//~for (i=0;i<list.length;i++) {
	//~print(list[i]);
//~}

t=205;
for (i=0;i<list.length;i++) { 
	//~t0 = getTime();

	open(dir1+ list[i]); 
	run("Subtract Background...","light"); 
	run("8-bit");
	//~makeOval(50, 50,900,900); //webcam
	makeOval(85, 95, 1370,1370); //for HD
	//~makeOval(30, 130, 1300,1300); //bubbles
	run("Set Measurements...", "area mean standard min center integrated limit display redirect =[None] decimal =[3]");
	resetThreshold();
	setThreshold(0,t);
	run("Threshold...");
	run("Analyze Particles...", "size=[15 - 100000] show=[Masks]  clear" );
	run("Watershed");
	run("Analyze Particles...", "size=[15 - 1000] circularity=[0.3 – 1.0] show=[Bare Outlines] clear summarize");
	resetThreshold();
	run("Close All");
//		t2 = getTime();
	//~a[i+1] = getTime();

}

//~for (i=0;i<list.length+1;i++) {
	//~print(a[i]);
//~}

//Array.print(a);
//run("Close All");
//run("Close All");
//times
// [1] 5270 1219 1517 2178  585 2675 3529 4297  767  335 5242  397  596  636  774
//[16]  922 1120 1364 1533 1858 2121 2445 2685 3125 3388 3770 4279 4667
//images:
//Mont100.tif	Mont16.tif	Mont25.tif	Mont36.tif	Mont4.tif	Mont49.tif	Mont64.tif	Mont81.tif	Mont9.tif	Scale100.tif	Scale1000.tif	Scale150.tif	Scale200.tif	Scale250.tif	//Scale300.tif	Scale350.tif	Scale400.tif	Scale450.tif	Scale500.tif	Scale550.tif	Scale600.tif	Scale650.tif	Scale700.tif	Scale750.tif	Scale800.tif	Scale850.tif	Scale900.tif	Scale950.tif	
//~

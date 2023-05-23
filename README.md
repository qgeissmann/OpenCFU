OpenCFU
======

What is Dockerfile
-------
[Docker](https://www.docker.com/) is a virtualization technology, allowing users to use containers to build projects with cross compatibility across different computers. Please install it to use OpenCFU.

You can build an example project with this: [mosesapostol1/opencfudocker](https://hub.docker.com/r/mosesapostol1/opencfudocker)

Then:

```
docker run -it moseapostol1/opencfudocker:v1
./opencfu <PATH NAME OF IMAGE>
```

You don't even need to compile the project anymore, the Dockerfile handles it.

Notes
-------
We are still testing this, please make an issue if this doesn't work. `Gtkmm` is broken - will be trying to port over the gui. This Docker image makes the CLI only work. Please see notes.



What is OpenCFU
-------
OpenCFU is a `C++` program to count bacterial colonies and other circular objects.
It heavily uses `OpenCV` for image processing and `Gtkmm` for GUI.
More information is available on the [website](http://www.opencfu.sourceforge.net).
For a more technical description, you can read the article published 
on [PLoS ONE](http://tinyurl.com/o3bk24o).

Installation
-------
http://opencfu.sourceforge.net/devcorner.php

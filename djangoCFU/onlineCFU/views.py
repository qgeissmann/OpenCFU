from django.shortcuts import render
from django.http import HttpResponse
from django.template import RequestContext, loader
from subprocess import Popen, PIPE
import json

from django.shortcuts import render_to_response
from django.utils import timezone
from .models import Images

from .forms import UploadFileForm

class OpenCFUParser:
	def __init__(self):
		self.result = []
		
		
	def run(self,args):
		argu = ["opencfu"]
		for i in args:
			argu.append(i)
			if args[i] != "":
				argu.append(args[i])
		try:
			p = Popen(argu, stdout=PIPE, stderr=PIPE, stdin=PIPE)
			ocfu_res = [i.decode().rstrip().split(",") for i in p.stdout]
			#~ dta = []
			#~ for i in range(1,len(ocfu_res)):
				#~ print ("test")
				#~ print ([j for j in ocfu_res[i]]) 
			self.result ={"head" : ocfu_res[0],'data' : [[float(j) for j in ocfu_res[i]] for i in range(1,len(ocfu_res))]}
			#~ print(self.result["data"])
			
			
		except:
			print ("oups something bad hapended")
			self.result =[]
	def getResultAsJson(self):
		out = json.dumps(self.result)
		return out

ocfu = OpenCFUParser()
#~ def counting(request):




def counting(request):
	print("counting")
	result = "[]"
	if request.method == 'POST':
		form = UploadFileForm(request.POST,request.FILES)
		image = Images(img = request.FILES['file'], submited_date = timezone.now())
		image.save()
		if form.is_valid():
			print("valid form")
			dic = form.makeArgs()
			dic["-i"] = image.img.path
			ocfu.run(dic)
			result = ocfu.getResultAsJson()
	return HttpResponse(result, content_type="application/json")
	




def index(request):
	template = loader.get_template('onlineCFU/index.html')
	res = ''
	form = UploadFileForm()

	#~ if request.method == 'POST':
		#~ and request.FILES['file']:
		#~ form = UploadFileForm(request.POST,request.FILES)
		#~ image = Images(img = request.FILES['file'], submited_date = timezone.now())
		#~ image.save()
		#~ if form.is_valid():
			#~ print("valid form")
			#~ dic = form.makeArgs()
			#~ dic["-i"] = image.img.path
			#~ ocfu.run(dic)
			
	#~ res = ocfu.getResultAsJson()
	context = RequestContext(request, {"form":form,})

	return HttpResponse(template.render(context))


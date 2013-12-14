from django import forms

class UploadFileForm(forms.Form):
	d = forms.MultipleChoiceField(label="Threshold type",choices=(("reg","regular"),("inv","inverted"),("bil","bilateral")), initial ="reg" ,help_text="blablabla")
	a = forms.BooleanField(label="Auto-Threshold", help_text="help_text", initial = True, required=False)
	t = forms.IntegerField(label="Threshold", help_text="help_text",min_value=0,max_value=255, initial = 5, required=True)
	r = forms.IntegerField(label="Min radius", help_text="help_text",min_value=0,max_value=99999, initial = 1, required=True)	 
	file  = forms.FileField()
	
	def makeArgs(self):
		dic = dict()
		if self.cleaned_data['a']:
			dic["-a"] = ""
		else:
			dic["-t"] = str(self.cleaned_data['t'])
		dic["-d"] = self.cleaned_data['d'][0]
		dic["-r"] = str(self.cleaned_data['r'])
		
		
		return dic

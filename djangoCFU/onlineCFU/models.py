from django.db import models

class Images(models.Model):
    #~ md5 = models.CharField(max_length=20)
    img = models.FileField(upload_to='images-stock')
    #~ ip_client = models.CharField(max_length=20)
    #~ local_path = models.CharField(max_length=50)
    submited_date = models.DateTimeField('date published')
    

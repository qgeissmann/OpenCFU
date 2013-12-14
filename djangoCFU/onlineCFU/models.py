from django.db import models
import uuid

def get_file_path(instance, filename):
    ext = filename.split('.')[-1]
    filename = "%s.%s" % (uuid.uuid4(), ext)
    return os.path.join('images-stock', filename)
    
class Images(models.Model):
    img = models.FileField(upload_to = get_file_path)
    submited_date = models.DateTimeField('date published')
    

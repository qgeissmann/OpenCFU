from django.conf.urls import patterns, include, url
from django.contrib import admin
from onlineCFU import views

admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    #+url(r'^$', 'djangoCFU.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
    
	url(r'^$', views.index, name='index'),
	url(r'^counting/$', views.counting, name='counting'),
	url(r'^uploadImage/$', views.uploadImage, name='uploadImage'),
    url(r'^admin/', include(admin.site.urls))
)

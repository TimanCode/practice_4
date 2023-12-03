from django.contrib import admin
from django.urls import path
from shortener.views import shorten_url, RedirectView
from stats_app.views import report_stats

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', shorten_url, name='shorten_url'),
    path('report/', report_stats, name='report_stats'),
    path('<str:short_url>', RedirectView.as_view(), name='redirect')
]
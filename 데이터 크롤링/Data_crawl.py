import csv, codecs
import urllib
import datetime
import time
from urllib.request import urlopen
from bs4 import BeautifulSoup
import matplotlib.pyplot as plt

with codecs.open("samsung.csv", "w", "euc_kr ") as fp:
    writer = csv.writer(fp, delimiter=",", quotechar='"')
    writer.writerow(["date", "price", "sixty"])

stockItem = '005930'

url = 'http://finance.naver.com/item/sise_day.nhn?code='+ stockItem

html = urlopen(url)
source = BeautifulSoup(html.read(), "html.parser")
maxPage=source.find_all("table",align="center")
mp = maxPage[0].find_all("td",class_="pgRR")
mpNum = int(mp[0].a.get('href')[-3:])


for page in range(45, 125):
    url = 'http://finance.naver.com/item/sise_day.nhn?code=' + stockItem +'&page='+ str(page)
    html = urlopen(url)
    source = BeautifulSoup(html.read(), "html.parser")
    srlists=source.find_all("tr")
    isCheckNone = None

    if((page % 1) == 0):
        time.sleep(0.5)
    for i in range(1,len(srlists)-1):
        if(srlists[i].span != isCheckNone):
            srlists[i].td.text
            """
print(srlists[i].find_all("td",align="center")[0].text
,srlists[i].find_all("td",class_="num")[0].text
"""
            print(
                srlists[i].find_all("td",align="center")[0].text.replace(",","")
                , srlists[i].find_all("td",class_="num")[0].text.replace(",","")
                )

            with codecs.open("samsung.csv", "a", "euc_kr ") as fp:
                writer = csv.writer(fp, delimiter=",", quotechar='"')
                writer.writerow([
                    srlists[i].find_all("td",align="center")[0].text.replace(",","")
                    , srlists[i].find_all("td",class_="num")[0].text.replace(",","")
                  
                    ])




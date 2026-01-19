import csv
import requests
import re
import json
import os
import subprocess
from tqdm import tqdm
from lxml import etree

class Video:
    def __init__(self,cookies,BVD):
        self.cookies = cookies
        self.BVD = BVD
        self.headers = {
            'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7',
            'accept-language': 'zh-CN,zh;q=0.9',
            'cache-control': 'max-age=0',
            'priority': 'u=0, i',
            'referer': 'https://www.bilibili.com/',
            'sec-ch-ua': '"Google Chrome";v="143", "Chromium";v="143", "Not A(Brand";v="24"',
            'sec-ch-ua-mobile': '?0',
            'sec-ch-ua-platform': '"Windows"',
            'sec-fetch-dest': 'document',
            'sec-fetch-mode': 'navigate',
            'sec-fetch-site': 'same-origin',
            'sec-fetch-user': '?1',
            'upgrade-insecure-requests': '1',
            'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/143.0.0.0 Safari/537.36',
            'cookie': self.cookies
        }

    def Response(self,url):
        """
        请求url

        :param url: 目标视频url
        :return response: 返回response对象
        """
        return requests.get(url,headers=self.headers,stream=True)

    @staticmethod
    def Progress(res,path,desc,block_size=1024):
        """
        实时显示下载进度

        :param res: 相应内容
        :param path: 文件地址
        :param desc: 下载内容
        :param block_size 分块内容大小
        :return: None
        """
        total_size = int(res.headers.get('content-length',0))
        with open(path,'wb') as f1,tqdm(total=total_size,unit='B',unit_scale=True,desc=f'{desc}下载：') as pbar:
            for data in res.iter_content(block_size):
                f1.write(data)
                pbar.update(len(data))
        print(f'{desc}下载成功')

    def download_resource(self):
        """
        下载音视频内容并合并

        :return: None
        """
        url = f'https://www.bilibili.com/video/{self.BVD}/?spm_id_from=333.1007.tianma.3-3-9.click&vd_source=e4853dea54f10a0668b6dce533b67e44'
        html = self.Response(url).text

        total_data = []
        # 获取标题
        title = re.search(r'<title>(.*?)_哔哩哔哩_bilibili</title>', html).group(1)
        # 逐个替换特殊字符
        replacements = {
            ':': '：',  # 中文冒号
            '"': '"',  # 左双引号
            '"': '"',  # 右双引号
            "'": ''',   # 左单引号
                        "'": ''',  # 右单引号
            '/': '／',  # 全角斜杠
            '\\': '＼',  # 全角反斜杠
            '|': '｜',  # 全角竖线
            '?': '？',  # 全角问号
            '*': '＊',  # 全角星号
            '<': '＜',  # 全角小于号
            '>': '＞',  # 全角大于号
        }
        for old, new in replacements.items():
            title = title.replace(old, new)
        title = title.strip()
        total_data.append(title)

        # 获取作者信息
        author = re.findall(r'"author":{(.*?)}',html)[1]
        author_str = json.loads('{' + f'{author}' + '}')
        author_url = f'https://space.bilibili.com/{author_str['mid']}?spm_id_from=333.788.upinfo.detail.click'
        total_data.extend((author_str['name'],author_url))

        # 获取时间
        xpath_html = etree.HTML(html)
        publish_time = xpath_html.xpath('//div[@class="pubdate-ip-text"]/text()')[0]
        total_data.append(publish_time)

        introduce_data = re.search(r'<script>window.__INITIAL_STATE__=(.+?)</script>',html).group(1)
        # 获取播放量、弹幕数、点赞数、投币数、收藏数、转发数、评论数
        first_data = re.search(r'"stat":{(.*?)}',introduce_data).group(0)
        first_data_str = json.loads('{' + f'{first_data}'+'}')
        total_data.extend((first_data_str['stat']['view'],first_data_str['stat']['danmaku'],first_data_str['stat']['like'],first_data_str['stat']['coin'],first_data_str['stat']['favorite'],first_data_str['stat']['share'],first_data_str['stat']['reply']))
        # 获取标签
        second_data = re.search(r'"tags":\[(.*?)\]',introduce_data).group(0)
        second_data_str = json.loads('{' + f'{second_data}' + '}')['tags']
        tag_list = []
        for _ in second_data_str:
            tag_list.append(_['tag_name'])
        total_data.append(tag_list)

        # 获取音视频url
        ResourceData = re.search(r'window.__playinfo__=(.*?)</script>',html)
        resource = json.loads(ResourceData.group(1))
        VideoUrl = resource['data']['dash']['video'][0]['baseUrl']
        AudioUrl = resource['data']['dash']['audio'][0]['baseUrl']
        total_data.extend((VideoUrl,AudioUrl))

        column_title = ['标题','作者','作者链接','发布时间','播放量','弹幕数','点赞数','投币数','收藏数','转发数','评论数','标签','视频链接','音频链接']
        with open('data.csv','w',encoding='utf-8-sig',newline='') as f:
            mid = csv.writer(f)
            mid.writerow(column_title)
            mid.writerow(total_data)

        os.makedirs('.\\视频',exist_ok=True)

        # 下载音频
        AudioResponse = self.Response(AudioUrl)
        audio_path = os.path.join('.\\视频',f'{title}.mp3')
        Video.Progress(AudioResponse,audio_path,"音频")

        # 下载视频
        VideoResponse = self.Response(VideoUrl)
        video_path = os.path.join('.\\视频',f'{title}.mp4')
        Video.Progress(VideoResponse,video_path,"视频")

        # 合并
        subprocess.run(f'ffmpeg -y -i .\\视频\\{title}.mp4 -i .\\视频\\{title}.mp3 -c:v copy -c:a aac -shortest {title}.mp4')

    def run(self):
        """
        主函数

        :return: None
        """
        self.download_resource()

if __name__ == '__main__':
    config = {
        'cookies': "buvid3=6AB46122-1114-4AF1-85E4-F05513CF002C94203infoc; b_nut=1761920194; _uuid=A96D8AA8-466B-9D43-EE6A-5B17F597CE31097390infoc; CURRENT_QUALITY=0; buvid_fp=7dc53d52907651242caea95b45648e16; buvid4=F109896E-D277-D4B7-B170-E22348F3794901450-025103122-3s2TtWTpGnrozTZqcWf3JA%3D%3D; rpdid=|(J~lu)YJYkl0J'u~Yumk|k)|; theme-tip-show=SHOWED; theme-avatar-tip-show=SHOWED; home_feed_column=5; DedeUserID=3546580405717300; DedeUserID__ckMd5=443ce1ae9dd8064c; bmg_af_switch=1; bmg_src_def_domain=i1.hdslb.com; browser_resolution=1707-932; bili_ticket=eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3Njg5OTU1MzEsImlhdCI6MTc2ODczNjI3MSwicGx0IjotMX0.stfFhTjFbdWo4D3C0WbfyJZjDW519p5hwX44a1W_Mvk; bili_ticket_expires=1768995471; SESSDATA=3acb30e5%2C1784288332%2C0dd79%2A11CjCpizgMKVCDxFk8EJ_hKIv-3bS2ash0tL410-XsMMV5j-FVeQR-G-QgU9oogoN7CtkSVmhpUXlFOWlmX1dYd2NqWXpZb2RxeVFocHRQUU5XM2ZLenUxVEJPd1pCWXdwVVNsYUNKc0RJbmFvNnM1elh6eHc5c0tPNE1DcVBtTHlWdVVGWFpQc29nIIEC; bili_jct=448b5238687d969eeed492c7adb0f258; sid=83zmhxro; CURRENT_FNVAL=4048; b_lsid=37E45E99_19BD1397D09",
        'BVD': "BV1ir421E78S"
    }
    start = Video(config['cookies'],config['BVD'])
    start.run()
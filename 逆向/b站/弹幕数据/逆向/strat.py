import requests
import execjs
import re

class Spider:

    def __init__(self,cookies,BV):

        with open('reverse.js','r',encoding='utf-8') as f:
            self.js_compile = execjs.compile(f.read())
        self.cookies = cookies
        self.headers = {
            'accept': 'application/json, text/plain, */*',
            'accept-language': 'zh-CN,zh;q=0.9',
            'origin': 'https://www.bilibili.com',
            'priority': 'u=1, i',
            'referer': 'https://www.bilibili.com/video/BV1hS4y1S7wL/?spm_id_from=333.788.player.player_end_recommend&vd_source=e4853dea54f10a0668b6dce533b67e44&trackid=web_related_0.router-related-2206146-j2qzm.1764833037396.284',
            'sec-ch-ua': '"Chromium";v="142", "Google Chrome";v="142", "Not_A Brand";v="99"',
            'sec-ch-ua-mobile': '?0',
            'sec-ch-ua-platform': '"Windows"',
            'sec-fetch-dest': 'empty',
            'sec-fetch-mode': 'cors',
            'sec-fetch-site': 'same-site',
            'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36'
        }
        self.BV = BV

    def getId(self):

        headers = {
            'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7',
            'accept-language': 'zh-CN,zh;q=0.9',
            'cache-control': 'max-age=0',
            'priority': 'u=0, i',
            'referer': 'https://search.bilibili.com/all?keyword=%E5%AE%8B%E6%B5%A9&from_source=webtop_search&spm_id_from=333.1007&search_source=5',
            'sec-ch-ua': '"Chromium";v="142", "Google Chrome";v="142", "Not_A Brand";v="99"',
            'sec-ch-ua-mobile': '?0',
            'sec-ch-ua-platform': '"Windows"',
            'sec-fetch-dest': 'document',
            'sec-fetch-mode': 'navigate',
            'sec-fetch-site': 'same-origin',
            'sec-fetch-user': '?1',
            'upgrade-insecure-requests': '1',
            'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36',
        }
        params = {
            'spm_id_from': '333.337.search-card.all.click',
        }
        response = requests.get('https://www.bilibili.com/video/BV1Cr4y1J7iQ/', params=params, cookies=self.cookies,headers=headers)
        pid = re.search(r'"cid"\s*:\s*(\d+)', response.text).group(1)
        oid = re.search(r'"aid"\s*:\s*(\d+)', response.text).group(1)
        return {
            'oid': oid,
            'pid': pid
        }

    @staticmethod
    def prepareParam(Id):

        return {
            'type': '1',
            'oid': Id['oid'],
            'pid': Id['pid'],
            'web_location': '1315873'
        }

    def crawlBulletComment(self,param):

        param['w_rid'] = self.js_compile.call('get_param')
        response = requests.get('https://api.bilibili.com/x/v2/dm/wbi/web/seg.so', params=param, cookies=self.cookies,headers=self.headers)


    def run(self):

        Id = self.getId()
        param = Spider.prepareParam(Id)
        self.crawlBulletComment(param)


if __name__ == '__main__':

    config = {
        'url': '',
        'cookies': ''
    }
    start = Spider(config['url'],config['cookies'])
    start.run()
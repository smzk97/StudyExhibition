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
            'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36',
            'cookie': self.cookies
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
            'cookies': self.cookies
        }
        params = {
            'spm_id_from': '333.337.search-card.all.click',
        }
        response = requests.get(f'https://www.bilibili.com/video/{self.BV}', params=params,headers=headers)
        oid = re.search(r'"cid"\s*:\s*(\d+)', response.text).group(1)
        pid = re.search(r'"aid"\s*:\s*(\d+)', response.text).group(1)
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

        # 第一次请求
        param['segment_index'] = '1'
        param['pull_mode'] = '1'
        param['ps'] = '0'
        param['pe'] = '120000'
        mid = self.js_compile.call('get_param',param['oid'],param['pid'],1)
        param['w_rid'] = mid['w_rid']
        param['wts'] = mid['wts']
        response = requests.get(url='https://api.bilibili.com/x/v2/dm/wbi/web/seg.so', params=param, headers=self.headers)
        print('第1次请求成功')
        print('-'*50)
        data_list = re.findall(r'([0-9\uFF10-\uFF19\u4e00-\u9fa5\u3000-\u303F\uFF00-\uFFEFa-zA-Z \u3000]+)@', response.text)
        for data in data_list:
            with open('1.txt','a',encoding='utf-8') as f:
                f.write(f'{data}\n')

        # 第二次请求
        param['ps'] = '120000'
        param['pe'] = '360000'
        mid = self.js_compile.call('get_param', param['oid'], param['pid'], 2)
        param['w_rid'] = mid['w_rid']
        param['wts'] = mid['wts']
        response = requests.get('https://api.bilibili.com/x/v2/dm/wbi/web/seg.so', params=param, headers=self.headers)
        print('第2次请求成功')
        print('-' * 50)
        data_list = re.findall(r'([0-9\uFF10-\uFF19\u4e00-\u9fa5\u3000-\u303F\uFF00-\uFFEFa-zA-Z \u3000]+)@',response.text)
        for data in data_list:
            with open('1.txt', 'a', encoding='utf-8') as f:
                f.write(f'{data}\n')

        # n次请求
        param.pop('pull_mode')
        param.pop('ps')
        param.pop('pe')
        index = 2
        while True:
            param['segment_index'] = f'{index}'
            index += 1
            mid = self.js_compile.call('get_param', param['oid'], param['pid'], 3)
            param['w_rid'] = mid['w_rid']
            param['wts'] = mid['wts']
            try:
                response = requests.get('https://api.bilibili.com/x/v2/dm/wbi/web/seg.so', params=param,headers=self.headers)
                response.raise_for_status()
                print(f'第{index+1}次请求成功')
                print('-' * 50)
                if not response.text:
                    break
                data_list = re.findall(r'([0-9\uFF10-\uFF19\u4e00-\u9fa5\u3000-\u303F\uFF00-\uFFEFa-zA-Z \u3000]+)@',response.text)
                for data in data_list:
                    with open('1.txt', 'a', encoding='utf-8') as f:
                        f.write(f'{data}\n')
            except:
                break

    def run(self):

        Id = self.getId()
        param = Spider.prepareParam(Id)
        self.crawlBulletComment(param)


if __name__ == '__main__':

    config = {
        'BV': 'BV1FGURBCE8H',
        'cookies': "buvid3=6AB46122-1114-4AF1-85E4-F05513CF002C94203infoc; b_nut=1761920194; _uuid=A96D8AA8-466B-9D43-EE6A-5B17F597CE31097390infoc; CURRENT_QUALITY=0; buvid_fp=7dc53d52907651242caea95b45648e16; buvid4=F109896E-D277-D4B7-B170-E22348F3794901450-025103122-3s2TtWTpGnrozTZqcWf3JA%3D%3D; rpdid=|(J~lu)YJYkl0J'u~Yumk|k)|; theme-tip-show=SHOWED; theme-avatar-tip-show=SHOWED; home_feed_column=5; browser_resolution=1920-945; DedeUserID=3546580405717300; DedeUserID__ckMd5=443ce1ae9dd8064c; CURRENT_FNVAL=4048; SESSDATA=295d2725%2C1780384945%2C71637%2Ac1CjCCa8-QEnWx8aKAa_QEa54yRqNsljwjuwz5yLufp1S-1ICFLUcofpexpFb2LBE0it0SVldnMEdrNzZGSlg4OEVvdnhZNHJ3bVdBbDdzRUgtVGdkYkM5NHY1SXNFU3J1aHdoQmw4YldkYkRBZGJFZ1JNMlNNTURKRVJGOVhVZFBiWkVTOUF5aWtnIIEC; bili_jct=e363e72e68219a168099b7320dd372e1; bp_t_offset_3546580405717300=1142507617663320064; b_lsid=F10EB7972_19AE9DF5FAB; sid=8nkai4gi; bili_ticket=eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3NjUxMTk1MjIsImlhdCI6MTc2NDg2MDI2MiwicGx0IjotMX0.qYGF06mx5ETjzkKPBoR3ZM5Hhi0QReT3aaQmErivzvM; bili_ticket_expires=1765119462"
    }
    start = Spider(config['cookies'],config['BV'])
    start.run()
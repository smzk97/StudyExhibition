import random
import time
import pandas as pd
import logging
import requests
from logging.handlers import RotatingFileHandler
import execjs
from datetime import datetime

# ----------------------
# logging配置
# ----------------------
logger = logging.getLogger('SpiderLogger')
logger.setLevel(logging.INFO)
handler = RotatingFileHandler('spider.log',maxBytes=5*1024*1024,backupCount=3,encoding='utf-8')
formatter = logging.Formatter("[%(asctime)s] [%(levelname)s] %(message)s")
handler.setFormatter(formatter)
logger.addHandler(handler)

class Spider:

    def __init__(self,main_oid,csv_path):
        """
        初始化

        :param main_oid : 每个视频唯一的oid编号
        :type main_oid : int
        :param csv_path : 要保存的csv文件地址
        :type csv_path : str
        """
        self.url_main = 'https://api.bilibili.com/x/v2/reply/wbi/main'
        self.main_oid = main_oid
        self.url_reply = 'https://api.bilibili.com/x/v2/reply/reply'
        self.csv_path = csv_path
        self.done = self.read_done()
        self.headers = {
    'accept': '*/*',
    'accept-language': 'zh-CN,zh;q=0.9',
    'origin': 'https://www.bilibili.com',
    'priority': 'u=1, i',
    'referer': 'https://www.bilibili.com/video/BV1b4ycBfEE8/?spm_id_from=333.1007.tianma.3-1-7.click&vd_source=e4853dea54f10a0668b6dce533b67e44',
    'sec-ch-ua': '"Chromium";v="142", "Google Chrome";v="142", "Not_A Brand";v="99"',
    'sec-ch-ua-mobile': '?0',
    'sec-ch-ua-platform': '"Windows"',
    'sec-fetch-dest': 'empty',
    'sec-fetch-mode': 'cors',
    'sec-fetch-site': 'same-site',
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36',
}
        with open('1.js','r',encoding='utf-8') as f:
            self.js_compile = execjs.compile(f.read())

    @staticmethod
    def _log(message):
        """
        控制台输出

        :param message : 要输出到控制台的信息
        :type message : str
        """
        print('-'*50,message,'-'*50)

    def read_done(self):
        """
        读取已爬取的id,页数，游标

        :return page : 上一次爬取的页数
        :rtype page : int
        :return pagination_str : 游标
        :rtype pagination_str : str
        """
        try:
            df = pd.read_csv(self.csv_path)
            page = df['页数'].astype(str).tolist()[-1]
            ids = df['rpid'].astype(str).tolist()
            pagination_str = df['pagination_str'].tolist()[-1]
            logger.info(f'已加载{len(ids)} 个已爬ID，最后一页是第{page}页，上一页游标是{pagination_str}')
            Spider._log(f'已加载{len(ids)} 个已爬ID，最后一页是第{page}页，上一页游标是{pagination_str}')
            return page,pagination_str

        except FileNotFoundError:
            logger.info("CSV 文件不存在，将创建新文件")
            Spider._log("CSV 文件不存在，将创建新文件")
            empty_csv = pd.DataFrame(columns=['用户名','等级','评论内容','日期','点赞数','页数','parent_id','rpid','oid','pagination_str'])
            empty_csv.to_csv(self.csv_path,index=False,encoding='utf-8-sig')
            return 0,''

    @staticmethod
    def retry_request(max_retries=3):
        """
        装饰重试机制

        :param max_retries : 最大重试次数
        :type max_retries : int
        :return decorator : 闭包函数
        :rtype decorator : func
        """
        def decorator(func):
            def wrapper(self,param,page):
                for attempt in range(1,max_retries+1):
                    try:
                        res = func(self,param,page)
                        res.raise_for_status()
                        logger.info(f'第{page}页请求成功')
                        Spider._log(f'第{page}页请求成功')
                        return res
                    except requests.exceptions.RequestException as f:
                        if attempt < max_retries:
                            logger.info(f'请求第{page}页失败，第{attempt}次重试')
                            Spider._log(f'请求第{page}页失败，第{attempt}次重试')
                            time.sleep(2**attempt)
                        else:
                            Spider._log(f'第{page}页所有重试均失败')
                            logger.info(f'第{page}页所有重试均失败')
                            return None
            return wrapper
        return decorator

    def prepare_request_main_param(self,page,pagination_str=None):
        """
        准备主评论请求参数

        :param page : 主评论的页数
        :type page : int
        :param pagination_str : 下一页的offset游标
        :type pagination_str : str
        :return param : 主评论请求需要的参数
        :rtype param : str
        """
        data = self.js_compile.call('get_main_param',self.main_oid,page,pagination_str)
        if page == 1:
            param = f'?oid={self.main_oid}&type=1&mode=3&pagination_str=%7B%22offset%22:%22%22%7D&plat=1&seek_rpid=&web_location=1315875&w_rid={data["w_rid"]}&wts={data["wts"]}'
        else :
            param = f'?oid={self.main_oid}&type=1&mode=3&pagination_str=%7B%22offset%22:%22{pagination_str}%22%7D&plat=1&web_location=1315875&w_rid={data["w_rid"]}&wts={data["wts"]}'
        return param

    @retry_request()
    def crawl_comment(self,param,page):
        """
        爬取主评论 + 重试机制

        :param param : 发起请求需要的参数
        :type param : str
        :param page : 主评论的页数
        :type page : int
        :return res : 主评论的响应内容
        :rtype res : requests.Response
        """
        logger.info(f'正在向第{page}页发起请求')
        Spider._log(f'正在向第{page}页发起请求')
        res = requests.get(url=self.url_main+param,headers=self.headers)
        return res

    @retry_request()
    def crawl_reply(self,root,pp):
        """
        爬取回复列表 + 重试机制

        :param root : 主评论的rpid值
        :type root : int
        :param pp :
                pp[0] 主评论的页数
                pp[1] 主评论的某一条评论
                pp[2] 回复评论的页数
        :type pp : (int,int,int)
        :return res : 回复评论的响应内容
        :rtype res : requests.Response
        """
        params = {
            'oid': self.main_oid,
            'type': '1',
            'root': f'{root}',
            'ps': '10',
            'pn': f'{pp[2]}',
            'web_location': '333.788',
        }
        logger.info(f'正在向第{pp[0]}页的第{pp[1]}条评论的回复评论的第{pp[2]}页发起请求')
        Spider._log(f'正在向第{pp[0]}页的第{pp[1]}条评论的回复评论的第{pp[2]}页发起请求')
        res = requests.get(url=self.url_reply,params=params,headers=self.headers)
        return res

    def data_main_deal(self,res,page):
        """
        处理数据

        :param res : 响应内容
        :type res : requests.Response
        :param page : 页数
        :type page : int
        """
        for index,r in enumerate(res.json()['data']['replies']):
            data = {
            'user_name': r['member']['uname'],
            'level': r['member']['level_info']['current_level'],
            'comment': r['content']['message'],
            'datatime': datetime.fromtimestamp(r['ctime']).strftime('%Y-%m-%d %H:%M:%S'),
            'likes': r['like'],
            'page': page,
            'parent_id': r['parent'],
            'rpid': r['rpid'],
            'oid': r['oid'],
            'pagination_str':res.json()['data']['cursor']['pagination_reply'].get('next_offset')
        }
            self.save_to_csv([data],page,index+1)
            if r['replies']:
                pn = 1
                while True:
                    reply_data = self.crawl_reply(r['rpid'],(page,index+1,pn))
                    if reply_data:

                        # 保存数据
                        for index_,_ in enumerate(reply_data.json()['data']['replies']):
                            reply_to_csv = {
                                'user_name': _['member']['uname'],
                                'level': _['member']['level_info']['current_level'],
                                'comment': _['content']['message'],
                                'datatime': datetime.fromtimestamp(_['ctime']).strftime('%Y-%m-%d %H:%M:%S'),
                                'likes': _['like'],
                                'page': page,
                                'parent_id': _['parent'],
                                'rpid': _['rpid'],
                                'oid': _['oid'],
                                'pagination_str': res.json()['data']['cursor']['pagination_reply'].get('next_offset')
                            }
                            self.save_to_csv([reply_to_csv],index+1,index_+1)

                        # 判断是否继续爬取下一页
                        num = reply_data.json()['data']['page']['num']
                        size = reply_data.json()['data']['page']['size']
                        count = reply_data.json()['data']['page']['count']
                        if num * size < count:
                            pn += 1
                        else:
                            break
                    else:
                        break
                    time.sleep(random.uniform(1.0,3.0))

    def save_to_csv(self,data,page,index):
        """
        将数据写入csv文件

        :param data : 要写入csv文件的内容
        :type data : list
        :param page : 页数
        :type page : int
        :param index : 某一条评论
        :type index : int
        """
        logger.info(f'第{page}页的第{index}条评论正在写入csv文件')
        Spider._log(f'第{page}页的第{index}条评论正在写入csv文件')
        df = pd.DataFrame(data)
        df.to_csv(self.csv_path,mode='a',header=False,index=False,encoding='utf-8')
        logger.info(f'第{page}页的第{index}条评论写入成功')
        Spider._log(f'第{page}页的第{index}条评论写入成功')

    def run(self):
        """
        重写run方法

        """
        page = self.done[0] + 1
        pagination_str = self.done[1]
        is_end = False
        while not is_end:
            if page == 1:
                param = self.prepare_request_main_param(page)
            else:
                param = self.prepare_request_main_param(page,pagination_str)
            res = self.crawl_comment(param,page)
            if res:
                self.data_main_deal(res,page)
                pagination_str = res.json()['data']['cursor']['pagination_reply'].get('next_offset')
                is_end = res.json()['data']['cursor']['is_end']
                page += 1
            else:
                break
            time.sleep(random.uniform(1.0, 3.0))

if __name__ == '__main__':

    config = {
        'main_oid':115579969085266,   # 视频的oid值，可在README.md文件查看更多
        'csv_path': './1.csv'         # csv文件保存的位置
    }
    start = Spider(config['main_oid'],config['csv_path'])
    start.run()
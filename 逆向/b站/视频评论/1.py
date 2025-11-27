import time
import pandas as pd
import logging
import requests
from logging.handlers import RotatingFileHandler
import execjs

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

    def __init__(self,main_oid,pagination_str,csv_path):
        """
        初始化

        """
        self.url_main = 'https://api.bilibili.com/x/v2/reply/wbi/main'
        self.main_oid = main_oid
        self.pagination_str = pagination_str
        self.url_reply = 'https://api.bilibili.com/x/v2/reply/reply'
        self.csv_path = csv_path
        self.done_rid = self.read_page()
        with open('1.js','r',encoding='utf-8') as f:
            self.js_compile = execjs.compile(f.read())

    @staticmethod
    def _log(message):
        """
        控制台输出

        :return:
        """
        print('-'*50,message,'-'*50)

    def read_page(self):
        """
        读取已爬取的rid

        :return:
        """
        try:
            df = pd.read_csv(self.csv_path)
            page = df['页数'].astype(str).tolist()[-1]
            ids = df['rpid'].astyppe(str).tolist()
            logger.info(f'已加载{len(ids)} 个已爬ID，最后一页是第{page}页')
            Spider._log(f'已加载{len(ids)} 个已爬ID，最后一页是第{page}页')
            return page

        except FileNotFoundError:
            logger.info("CSV 文件不存在，将创建新文件")
            empty_csv = pd.DataFrame(columns=['用户名','等级','评论内容','日期','点赞数','页数','reply_to_id','parent_id','rpid','oid'])
            empty_csv.to_csv(self.csv_path,index=False)
            return 0

    @staticmethod
    def retry_request(max_retries=3):
        """
        装饰重试机制

        :return:
        """
        def decorator(func):
            def wrapper(param,page):
                for attempt in range(1,max_retries+1):
                    try:
                        res = func(param,page)
                        res.raise_for_status()
                        logger.info(f'第{page}页请求成功')
                        Spider._log(f'第{page}页请求成功')
                        return res
                    except requests.exceptions.RequestException as f:
                        if attempt <= max_retries:
                            logger.info(f'请求失败，第{attempt}次重试')
                            Spider._log(f'请求失败，第{attempt}次重试')
                            time.sleep(2**attempt)
                        else:
                            Spider._log('所有重试均失败')
                            return None
            return wrapper
        return decorator

    def prepare_request_param(self,page):
        """
        准备请求参数

        :return:
        """


    @retry_request()
    def crawl_comment(self,param,page):
        """
        爬取主评论 + 重试机制

        :return:
        """
        logger.info(f'正在向第{page}页发起请求')
        Spider._log(f'正在向第{page}页发起请求')
        res = requests.get(url=self.url_main,params=param)
        return res

    @retry_request()
    def crawl_reply(self,param,page):
        """
        爬取回复列表 + 重试机制

        :return:
        """
        logger.info(f'正在向第{page}页发起请求')
        Spider._log(f'正在向第{page}页发起请求')
        res = requests.get(url=self.url_reply,params=param)
        return res

    def data_deal(self,res,page):
        """
        处理数据

        :return:
        """
        return{
            'user_name': res['member']['uname'],
            'level': res['level_info']['current_level'],
            'comment': res['content']['message'],
            'datatime': res['ctime'],
            'likes': res['like'],
            'page': page,
            'reply_to_id': '',
            'parent_id': '',
            'rpid': res['rpid'],
            'oid': res['oid']
        }

    def save_to_csv(self,data,page):
        """
        将数据写入csv文件

        :return:
        """
        logger.info(f'第{page}页的内容正在写入csv文件')
        Spider._log(f'第{page}页的内容正在写入csv文件')
        df = pd.DataFrame(data)
        df.to_csv(self.csv_path,mode='a',header=False,index=False)
        logger.info(f'第{page}页的内容写入成功')
        Spider._log(f'第{page}页的内容写入成功')

    def run(self):
        """
        重写run方法

        :return:
        """
        page = self.read_page() + 1
        while True:



if __name__ == '__main__':
    start = Spider
    start.run()
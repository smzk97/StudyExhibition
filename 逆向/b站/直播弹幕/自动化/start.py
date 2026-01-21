from DrissionPage import Chromium
import re

class Danmu:

    def __init__(self,url):
        """
        实例初始化

        :param url: 目标网址
        :return: None
        """
        self.url = url
        self.tab = Chromium().latest_tab
        with open('DOM.js','r',encoding='utf-8') as f:
            self.js = f.read()

    def run_js(self):
        """
        执行监听任务

        :return: None
        """
        self.tab.get(self.url)
        chat_items = self.tab.ele('.chat-items', timeout=10)
        if not chat_items:
            print("未找到弹幕容器元素")
            return
        self.tab.console.start()
        self.tab.console.clear()
        self.tab.run_js(self.js)
        while True:
            text = self.tab.console.wait().text
            if re.match(r'弹幕内容',text):
                print(text)
                with open('data.txt','a',encoding='utf-8') as f:
                    f.read(text)

    def run(self):
        """
        主函数

        :return: None
        """
        self.run_js()

if __name__ == '__main__':
    config = {
        'url' : 'https://live.bilibili.com/7878?hotRank=0&session_id=6aad5e86b438c2246b34f8f81569710d_430F469F-044C-4AFF-828A-53E7C73B86E9&live_from=77002&trackid=-99998&visit_id=bj3vmpytie80',
    }
    start = Danmu(config['url'])
    start.run()
from DrissionPage import Chromium
class Automation:

    def __init__(self,url):

        self.url = url
        browser = Chromium()
        self.tab = browser.latest_tab
        with open('DOM.js','r',encoding='utf-8') as f:
            self.js_code = f.read()

    def preprocessing(self,url):

        self.tab.get(url)
        # 同步控制台
        self.tab.console.start()

        # 滑动进度条到起始位置，暂停视频
        self.tab.wait.eles_loaded('.bpx-player-progress-schedule-current')
        offset_x = self.tab.ele('.bpx-player-progress-schedule-current').rect.size[0]+14
        self.tab.wait.eles_loaded('.bpx-player-progress-thumb')
        self.tab.actions.hold('.bpx-player-progress-thumb').move(-offset_x,0).release()
        self.tab.wait.eles_loaded('.bpx-player-ctrl-btn bpx-player-ctrl-play')
        self.tab.ele('.bpx-player-ctrl-btn bpx-player-ctrl-play').click()

        # 打开弹幕
        if self.tab.wait.eles_loaded('.bpx-player-dm-setting disabled'):
            self.tab.wait.eles_loaded('.bui-danmaku-switch-input')
            self.tab.ele('.bui-danmaku-switch-input').click()
            self.tab.wait.eles_loaded('bui-checkbox-input')

    def monitor(self):

        self.tab.run_js(self.js_code)
        while True:
            comment = self.tab.console.wait().text
            with open('1.txt','a',encoding='utf-8') as f:
                f.write(f'{comment}\n')

    def run(self):
        self.preprocessing(self.url)

if __name__ == '__main__':

    config = {
        'url' : 'https://www.bilibili.com/video/BV1w91sBpEYL/?spm_id_from=333.1007.tianma.5-2-16.click&vd_source=e4853dea54f10a0668b6dce533b67e44'
    }
    start = Automation(config['url'])
    start.run()


from DrissionPage import Chromium
import time
class Automation:

    def __init__(self,url):
        """
        初始化

        :param url: 目标网址
        :type url: str
        """
        self.url = url
        browser = Chromium()
        self.tab = browser.latest_tab
        with open('DOM.js','r',encoding='utf-8') as f:
            self.js_code = f.read()

    def preprocessing(self,url):
        """
        回调函数执行之前的视频播放设置

        :param url: 目标网址
        :type url: str
        """
        self.tab.get(url)
        # 同步控制台
        self.tab.console.start()

        # 暂停视频
        self.tab.wait.eles_loaded('.bpx-player-ctrl-btn bpx-player-ctrl-play')
        self.tab.ele('.bpx-player-ctrl-btn bpx-player-ctrl-play').click()

        # 打开弹幕
        if self.tab.wait.eles_loaded('.bpx-player-dm-setting disabled'):
            self.tab.wait.eles_loaded('.bui-danmaku-switch-input')
            self.tab.ele('.bui-danmaku-switch-input').click()
            self.tab.wait.eles_loaded('bui-checkbox-input')

        # 开启3.5倍速
        rate = self.tab.ele('.bpx-player-ctrl-playbackrate-menu-item ')
        rate.set.attr('data-value', "3.5")
        rate.set.attr('class', 'bpx-player-ctrl-playbackrate-menu-item bpx-state-active')
        bt = self.tab.ele('.bpx-player-ctrl-btn bpx-player-ctrl-playbackrate')
        self.tab.actions.move_to(bt)
        rate.click()

        # 滑动进度条到初始位置
        self.tab.wait.eles_loaded('.bpx-player-progress-schedule-current')
        offset_x = self.tab.ele('.bpx-player-progress-schedule-current').rect.size[0]+14
        self.tab.wait.eles_loaded('.bpx-player-progress-thumb')
        self.tab.actions.hold('.bpx-player-progress-thumb').move(-offset_x,0).release()

    def monitor(self):
        """
        开始监听

        """
        self.tab.console.clear()
        self.tab.run_js(self.js_code)
        while True:
            comment = self.tab.console.wait().text
            print(comment)
            with open('1.txt','a',encoding='utf-8') as f:
                f.write(f'{comment}\n')
            current_time = list(map(int,self.tab.ele('.bpx-player-ctrl-time-current').text.split(':')))
            total_time = list(map(int,self.tab.ele('.bpx-player-ctrl-time-duration').text.split(':')))
            current_total = total = 0
            for index,_time in enumerate(current_time):
                current_total += _time * (60 ** (len(current_time) - 1 - index))
            for index,_time in enumerate(total_time):
                total += _time * (60 ** (len(total_time) - 1 - index))
            if current_total  == total:
                self.tab.close()
                break

    def run(self):
        """
        重写run方法

        """
        self.preprocessing(self.url)
        self.monitor()

if __name__ == '__main__':

    config = {
        'url' : 'https://www.bilibili.com/video/BV1w91sBpEYL/?spm_id_from=333.1007.tianma.5-2-16.click&vd_source=e4853dea54f10a0668b6dce533b67e44'
    }
    start = Automation(config['url'])
    start.run()



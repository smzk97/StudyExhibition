import csv
import requests
import time

class Danmu:
    def __init__(self,roomid,cookie):
        """
        定义类实例

        :param roomid : 直播间的唯一标识
        :param cookie: 身份验证
        :param return: None
        """
        self.roomid = roomid
        self.cookie = cookie
        self.headers = {
            'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/143.0.0.0 Safari/537.36',
            'cookies': self.cookie
        }

    def get_res(self):
        """
        返回请求对象的json格式数据

        :return: None
        """
        params = {
            'roomid': str(self.roomid),
            'room_type': '0',
        }
        response = requests.get(
            'https://api.live.bilibili.com/xlive/web-room/v1/dM/gethistory',
            params=params,
            headers=self.headers,
        )
        return response.json()

    def deal_data(self,res,set_data):
        """
        对弹幕内容进行去重与保存

        :param res: 返回的json格式数据
        :param set_data: 用于去重的集合
        :return: None
        """
        for _ in res['data']['room']:
            mid = {
                '评论内容': _['text'],
                '时间': _['timeline'],
                '用户名': _['nickname'],
                '用户空间链接': f'https://space.bilibili.com/{_['uid']}'
            }
            key = (_['text'],_['timeline'],_['nickname'])
            if key not in set_data:
                set_data.add(key)
                head = ['评论内容', '时间', '用户名', '用户空间链接']
                with open('data.csv', 'a', encoding='utf-8-sig', newline='') as f:
                    write = csv.DictWriter(f, fieldnames=head)
                    write.writerow(mid)
                    print(mid)

    def run(self):
        """
        主函数

        :return: None
        """
        data = set()
        head = ['评论内容','时间','用户名','用户空间链接']
        with open('data.csv', 'w', encoding='utf-8-sig', newline='') as f:
            write = csv.DictWriter(f,fieldnames=head)
            write.writeheader()
        while True:
            try:
                mid_data = self.get_res()
                self.deal_data(mid_data,data)
                time.sleep(2)
            except requests.exceptions.RequestException as exception:
                print(exception)
                break

if __name__ == '__main__':
    config = {
        'roomid': "545068",
        'cookie': "buvid3=6AB46122-1114-4AF1-85E4-F05513CF002C94203infoc; b_nut=1761920194; _uuid=A96D8AA8-466B-9D43-EE6A-5B17F597CE31097390infoc; CURRENT_QUALITY=0; buvid_fp=7dc53d52907651242caea95b45648e16; buvid4=F109896E-D277-D4B7-B170-E22348F3794901450-025103122-3s2TtWTpGnrozTZqcWf3JA%3D%3D; rpdid=|(J~lu)YJYkl0J'u~Yumk|k)|; theme-tip-show=SHOWED; theme-avatar-tip-show=SHOWED; home_feed_column=5; DedeUserID=3546580405717300; DedeUserID__ckMd5=443ce1ae9dd8064c; browser_resolution=1707-932; bp_t_offset_3546580405717300=1159596072763392000; bili_ticket=eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3NjkxNDc4MTUsImlhdCI6MTc2ODg4ODU1NSwicGx0IjotMX0.cpEsZxkNCTAyvYKJsdcFLcInxGu3Snkubkdh_kBhGmQ; bili_ticket_expires=1769147755; LIVE_BUVID=AUTO5317688886158468; CURRENT_FNVAL=4048; SESSDATA=f3359f26%2C1784548680%2Ccbb15%2A11CjCULYUDU8smHjzO7_O5sEasIm35DSoYyl83EKVt2x4bIFMcLLztqeoqdV_j69_hsXUSVlVQbnBXNTktY04yTDA3NnB6V0dVa0hocFFKb1p2VGtvSEZoYkVPc25QQWR2clRqTDdWYWoyNWI1ejJZOVRJektXY1lzLTFwLUdVdl9oYmhWUmM4NlNnIIEC; bili_jct=1cb7e3b2bca62a429c1d75e8f4377047; sid=q3elq7bb; b_lsid=CF61D4EF_19BE087D50E; PVID=11"
    }
    start = Danmu(config['roomid'],config['cookie'])
    start.run()


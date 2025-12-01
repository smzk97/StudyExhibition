import requests
import re
cookies = {
    'buvid3': '6AB46122-1114-4AF1-85E4-F05513CF002C94203infoc',
    'b_nut': '1761920194',
    '_uuid': 'A96D8AA8-466B-9D43-EE6A-5B17F597CE31097390infoc',
    'CURRENT_QUALITY': '0',
    'buvid_fp': '7dc53d52907651242caea95b45648e16',
    'buvid4': 'F109896E-D277-D4B7-B170-E22348F3794901450-025103122-3s2TtWTpGnrozTZqcWf3JA%3D%3D',
    'rpdid': "|(J~lu)YJYkl0J'u~Yumk|k)|",
    'theme-tip-show': 'SHOWED',
    'theme-avatar-tip-show': 'SHOWED',
    'home_feed_column': '5',
    'browser_resolution': '1920-945',
    'DedeUserID': '3546580405717300',
    'DedeUserID__ckMd5': '443ce1ae9dd8064c',
    'CURRENT_FNVAL': '4048',
    'SESSDATA': '9d3986e1%2C1779972912%2Cf23d1%2Ab1CjBPK1YjMb07vBX0RMtQ0Fpvhk7z6hEGewRlvoC8ACy2V6t3r30PReFUWMhOSUN60ZwSVjJkNTVWaVl1SngtNFRaWkZxazhDY1Bzd1NmdnhHTTdYV3R1QllLdW1lY3BEVnVPZV93S3NNc2pwYVlvay03WkR6RVFwOU5nbXRwWkFqVHFWSUtldnhRIIEC',
    'bili_jct': 'e505ef0c465cdb2592b4cbc758968952',
    'sid': '4h62oe0r',
    'bili_ticket': 'eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3NjQ3NjQ2NjYsImlhdCI6MTc2NDUwNTQwNiwicGx0IjotMX0.W_KGygIrXqau48X9Dy4iFHv4rLvqZK4eUsvM7PcxvaU',
    'bili_ticket_expires': '1764764606',
    'b_lsid': 'D399DA44_19AD99A4A17',
    'bp_t_offset_3546580405717300': '1141386652673900544',
}

headers = {
    'accept': 'application/json, text/plain, */*',
    'accept-language': 'zh-CN,zh;q=0.9',
    'origin': 'https://www.bilibili.com',
    'priority': 'u=1, i',
    'referer': 'https://www.bilibili.com/video/BV1z6aEzSEby/?spm_id_from=333.788.player.player_end_recommend_autoplay&vd_source=e4853dea54f10a0668b6dce533b67e44&trackid=web_related_0.router-related-2206146-j2qzm.1764509413031.258',
    'sec-ch-ua': '"Chromium";v="142", "Google Chrome";v="142", "Not_A Brand";v="99"',
    'sec-ch-ua-mobile': '?0',
    'sec-ch-ua-platform': '"Windows"',
    'sec-fetch-dest': 'empty',
    'sec-fetch-mode': 'cors',
    'sec-fetch-site': 'same-site',
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36',
    # 'cookie': "buvid3=6AB46122-1114-4AF1-85E4-F05513CF002C94203infoc; b_nut=1761920194; _uuid=A96D8AA8-466B-9D43-EE6A-5B17F597CE31097390infoc; CURRENT_QUALITY=0; buvid_fp=7dc53d52907651242caea95b45648e16; buvid4=F109896E-D277-D4B7-B170-E22348F3794901450-025103122-3s2TtWTpGnrozTZqcWf3JA%3D%3D; rpdid=|(J~lu)YJYkl0J'u~Yumk|k)|; theme-tip-show=SHOWED; theme-avatar-tip-show=SHOWED; home_feed_column=5; browser_resolution=1920-945; DedeUserID=3546580405717300; DedeUserID__ckMd5=443ce1ae9dd8064c; CURRENT_FNVAL=4048; SESSDATA=9d3986e1%2C1779972912%2Cf23d1%2Ab1CjBPK1YjMb07vBX0RMtQ0Fpvhk7z6hEGewRlvoC8ACy2V6t3r30PReFUWMhOSUN60ZwSVjJkNTVWaVl1SngtNFRaWkZxazhDY1Bzd1NmdnhHTTdYV3R1QllLdW1lY3BEVnVPZV93S3NNc2pwYVlvay03WkR6RVFwOU5nbXRwWkFqVHFWSUtldnhRIIEC; bili_jct=e505ef0c465cdb2592b4cbc758968952; sid=4h62oe0r; bili_ticket=eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3NjQ3NjQ2NjYsImlhdCI6MTc2NDUwNTQwNiwicGx0IjotMX0.W_KGygIrXqau48X9Dy4iFHv4rLvqZK4eUsvM7PcxvaU; bili_ticket_expires=1764764606; b_lsid=D399DA44_19AD99A4A17; bp_t_offset_3546580405717300=1141386652673900544",
}

params = {
    'type': '1',
    'oid': '32085183385',
    'pid': '115127135242768',
    'segment_index': '2',
    'web_location': '1315873',
    'w_rid': '65743c3bdca5df73cd972d8be390f445',
    'wts': '1764589239',
}

response = requests.get('https://api.bilibili.com/x/v2/dm/wbi/web/seg.so', params=params, cookies=cookies, headers=headers)
data = response.text
data_list = re.findall(r'([0-9\uFF10-\uFF19\u4e00-\u9fa5\u3000-\u303F\uFF00-\uFFEFa-zA-Z \u3000]+)@',data)
for i in data_list:
    print(i)
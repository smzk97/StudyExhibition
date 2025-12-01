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
    'SESSDATA': '9d3986e1%2C1779972912%2Cf23d1%2Ab1CjBPK1YjMb07vBX0RMtQ0Fpvhk7z6hEGewRlvoC8ACy2V6t3r30PReFUWMhOSUN60ZwSVjJkNTVWaVl1SngtNFRaWkZxazhDY1Bzd1NmdnhHTTdYV3R1QllLdW1lY3BEVnVPZV93S3NNc2pwYVlvay03WkR6RVFwOU5nbXRwWkFqVHFWSUtldnhRIIEC',
    'bili_jct': 'e505ef0c465cdb2592b4cbc758968952',
    'sid': '4h62oe0r',
    'bmg_af_switch': '1',
    'bmg_src_def_domain': 'i1.hdslb.com',
    'bili_ticket': 'eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3NjQ4NTY2MDMsImlhdCI6MTc2NDU5NzM0MywicGx0IjotMX0.qaKnp6ENV0LVJxnKcyLJE4URPiJTTAMGZjpUuI33a7U',
    'bili_ticket_expires': '1764856543',
    'CURRENT_FNVAL': '4048',
    'bp_t_offset_3546580405717300': '1141428197892554752',
    'b_lsid': '6109DD85B_19ADA3F376D',
}

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
    # 'cookie': "buvid3=6AB46122-1114-4AF1-85E4-F05513CF002C94203infoc; b_nut=1761920194; _uuid=A96D8AA8-466B-9D43-EE6A-5B17F597CE31097390infoc; CURRENT_QUALITY=0; buvid_fp=7dc53d52907651242caea95b45648e16; buvid4=F109896E-D277-D4B7-B170-E22348F3794901450-025103122-3s2TtWTpGnrozTZqcWf3JA%3D%3D; rpdid=|(J~lu)YJYkl0J'u~Yumk|k)|; theme-tip-show=SHOWED; theme-avatar-tip-show=SHOWED; home_feed_column=5; browser_resolution=1920-945; DedeUserID=3546580405717300; DedeUserID__ckMd5=443ce1ae9dd8064c; SESSDATA=9d3986e1%2C1779972912%2Cf23d1%2Ab1CjBPK1YjMb07vBX0RMtQ0Fpvhk7z6hEGewRlvoC8ACy2V6t3r30PReFUWMhOSUN60ZwSVjJkNTVWaVl1SngtNFRaWkZxazhDY1Bzd1NmdnhHTTdYV3R1QllLdW1lY3BEVnVPZV93S3NNc2pwYVlvay03WkR6RVFwOU5nbXRwWkFqVHFWSUtldnhRIIEC; bili_jct=e505ef0c465cdb2592b4cbc758968952; sid=4h62oe0r; bmg_af_switch=1; bmg_src_def_domain=i1.hdslb.com; bili_ticket=eyJhbGciOiJIUzI1NiIsImtpZCI6InMwMyIsInR5cCI6IkpXVCJ9.eyJleHAiOjE3NjQ4NTY2MDMsImlhdCI6MTc2NDU5NzM0MywicGx0IjotMX0.qaKnp6ENV0LVJxnKcyLJE4URPiJTTAMGZjpUuI33a7U; bili_ticket_expires=1764856543; CURRENT_FNVAL=4048; bp_t_offset_3546580405717300=1141428197892554752; b_lsid=6109DD85B_19ADA3F376D",
}

params = {
    'spm_id_from': '333.337.search-card.all.click',
}

response = requests.get('https://www.bilibili.com/video/BV1CAxaeHEeH/', params=params, cookies=cookies, headers=headers)
cid = re.search(r'"cid"\s*:\s*(\d+)',response.text)
aid = re.search(r'"aid"\s*:\s*(\d+)',response.text)
print(aid.group(1),cid.group(1))
# script_pattern = re.compile(r'<script.*?>[\s\S]*?"embedPlayer"[\s\S]*?</script>', re.IGNORECASE)
#
# # 3. 提取目标脚本内容
# script_matches = script_pattern.findall(response.text)
# if not script_matches:
#     print("未找到包含embedPlayer的<script>标签")
# else:
#     # 4. 从脚本内容中提取cid（适配"cid": 数字 格式，兼容空格）
#     cid_pattern = re.compile(r'"cid"\s*:\s*(\d+)')
#     all_cids = []
#     for script in script_matches:
#         cids = cid_pattern.findall(script)
#         all_cids.extend(cids)
#
#     if all_cids:
#         print("提取到的cid值：")
#         for cid in all_cids:
#             print(cid)  # 输出示例：26087195706
#     else:
#         print("脚本中未找到cid字段")
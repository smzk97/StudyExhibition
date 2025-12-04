let crypto = require('crypto');


function get_main_param(oid,pid,segment_index,label){
    let ts = Math.round(Date.now() / 1e3).toString();
    let v = ''
    if(label === 1)
        v = `'oid=${oid}&pid=${pid}&segment_index=${segment_index}&type=1&web_location=1315873&wts=${ts}'`
    else
        v = `mode=3&oid=${oid}&pagination_str=%7B%22offset%22%3A%22${pagination_str}%22%7D&plat=1&type=1&web_location=1315875&wts=${ts}`;
    let a = (t = "7cd084941338484aae1ad9425b84077c" + "4932caff0ff746eab6f01bf08b70ac45",
            r = [],
            [46, 47, 18, 2, 53, 8, 23, 32, 15, 50, 10, 31, 58, 3, 45, 35, 27, 43, 5, 49, 33, 9, 42, 19, 29, 28, 14, 39, 12, 38, 41, 13, 37, 48, 7, 16, 24, 55, 40, 61, 26, 17, 0, 1, 60, 51, 30, 4, 22, 25, 54, 21, 56, 59, 6, 63, 57, 62, 11, 36, 20, 34, 44, 52].forEach((function(e) {
                t.charAt(e) && r.push(t.charAt(e))
            }
            )),r.join("").slice(0, 32));
    return {
        'w_rid': crypto.createHash('md5').update(v + a).digest('hex'),
        'wts': ts
    }
}

console.log(get_main_param())

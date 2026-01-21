let startObserve = () => {
    let targetNode = document.querySelector('.chat-items');
    if (!targetNode) {
        console.log('等待容器加载');
        setTimeout(startObserve, 500);
        return;
    }
    let observerOptions = {
        subtree: true,
        childList: true
    }
    let key = new Set();
    let get_data = (node) => {
        return {
            'data-danmaku': node.getAttribute('data-danmaku'),
            'data-timestamp': node.getAttribute('data-timestamp'),
            'data-uname': node.getAttribute('data-uname'),
            'data-uid': node.getAttribute('data-uid')
        };
    }
    let callback = (mutationLists) => {
        for (let mutationList of mutationLists) {
            if (mutationList.type === 'childList') {
                mutationList.addedNodes.forEach((node) => {
                    let comment_data = get_data(node);
                    let mid_key = `${comment_data['data-uname']}_${comment_data['data-timestamp']}`;
                    if (!key.has(mid_key)) {
                        key.add(mid_key);
                        let date = new Date(comment_data['data-timestamp'] * 1000);
                        let formateTime = date.getFullYear() + '-' +
                            (date.getMonth() + 1).toString().padStart(2, '0') + '-' +
                            date.getDate().toString().padStart(2, '0') + ' ' +
                            date.getHours().toString().padStart(2, '0') + ':' +
                            date.getMinutes().toString().padStart(2, '0') + ':' +
                            date.getSeconds().toString().padStart(2, '0');
                        console.log(`弹幕内容：${comment_data['data-danmaku']}，发布时间：${formateTime}，用户：${comment_data['data-uname']}，用户空间：https://space.bilibili.com/${comment_data['data-uid']}`);
                    }
                })
            }
        }
    }
    let observer = new MutationObserver(callback);
    observer.observe(targetNode, observerOptions);
}
startObserve();
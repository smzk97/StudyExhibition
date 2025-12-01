from DrissionPage import Chromium

browser = Chromium()
tab = browser.latest_tab
tab.get('https://www.bilibili.com/video/BV1CAxaeHEeH/?spm_id_from=333.337.search-card.all.click&vd_source=e4853dea54f10a0668b6dce533b67e44')
tab.console.start()
tab.wait.eles_loaded('.bui-danmaku-switch-input')
tab.ele('.bui-danmaku-switch-input').click()
tab.wait.eles_loaded('.bpx-player-row-dm-wrap')

tab.run_js("console.clear();")

code = """
let element = document.querySelector('.bpx-player-row-dm-wrap');
let config = {
    childList:true,
    attributes:true,
    subtree:true,
};

let printedSet = new Set();

let callback = (mutationlist)=>{
    for (let mutation of mutationlist){
        if(mutation.type === 'childList'){
            mutation.addedNodes.forEach((node)=>{
                if (node.nodeName === "DIV" && node.nodeType === 1) {
                    if (printedSet.has(node.innerText))
                        return;
                    printedSet.add(node.innerText.trim());
                    console.log(node.innerText.trim());
                }
            })
        }
        if (mutation.type === 'attributes'){
            let targetNode = mutation.target;
            if (targetNode.nodeName === "DIV" && targetNode.nodeType === 1 ) {
                if (printedSet.has(targetNode.innerText))
                    return;
                printedSet.add(targetNode.innerText.trim());
                console.log(targetNode.innerText.trim());
            }
        }
    }
}

let observer = new MutationObserver(callback);
observer.observe(element, config);
"""
tab.run_js(code)

while True:
    message = tab.console.wait().text
    if message:
        print(message)
        with open('1.txt','a',encoding='utf-8') as f:
            f.write(f'{message}\n')
from DrissionPage import Chromium

browser = Chromium()
tab = browser.latest_tab
tab.get(
    'https://www.bilibili.com/video/BV1w91sBpEYL/?spm_id_from=333.1007.tianma.5-2-16.click&vd_source=e4853dea54f10a0668b6dce533b67e44')
tab.console.start()
tab.wait.eles_loaded('.bui-danmaku-switch-input')
tab.ele('.bui-danmaku-switch-input').click()
tab.wait.eles_loaded('.bpx-player-row-dm-wrap')

tab.run_js("console.clear();")

code = """
// 1. 获取监听目标容器
let element = document.querySelector('.bpx-player-row-dm-wrap');

// 2. 配置项：开启 subtree 以监听子元素变化
// 注意：虽然开启了 attributes，但我们在代码里会过滤掉 style
let config = {
    childList: true,
    attributes: true,
    subtree: true,
    characterData: true // 增加这个，以防 B 站直接修改文本节点内容
};

/**
 * 辅助函数：安全获取节点的纯文本
 * 处理直接包含文本或嵌套 DIV 的情况
 */
const getDanmakuText = (node) => {
    // 使用 textContent 性能更好，且能获取所有后代文本
    return node.textContent ? node.textContent.trim() : "";
};
// ... 配置和 getDanmakuText (或直接使用 targetNode.textContent) 保持不变 ...

let callback = (mutationList) => {

    for (let mutation of mutationList) {

        // ----------------------------------------------------
        // A. 阶段一：处理初始加载 (childList - 捕获新增节点)
        // ----------------------------------------------------
        if (mutation.type === 'childList') {

            // **核心过滤器：** 排除针对最外层容器的变动，只处理新增的子节点
            // 否则会捕获到 S_full
            if (mutation.target.className.includes('bpx-player-row-dm-wrap')) {
                // 遍历新增的节点，这些就是单个的弹幕行 DIV
                mutation.addedNodes.forEach((node) => {
                    // 确保是 Element 节点 (即弹幕行 DIV)
                    if (node.nodeType === 1) { 
                        // **使用 V3.0 的去重逻辑**：因为这里的 node 是新的，只需捕获即可
                        let currentText = node.textContent.trim();
                        if (currentText && node._lastSeenText !== currentText) {
                            node._lastSeenText = currentText;
                            console.log(`[捕获 - ChildList]`, currentText);
                        }
                    }
                });
            }
            // childList 记录处理完毕，跳过后续的 attributes/characterData 检查，避免重复捕获
            continue; 
        }

        // ----------------------------------------------------
        // B. 阶段二：处理复用更新 (attributes / characterData)
        // ----------------------------------------------------

        // 1. 【噪音过滤】忽略高频 style 变动 (Attributes 记录)
        if (mutation.type === 'attributes' && mutation.attributeName === 'style') {
            continue;
        }

        // 2. 【目标归一化】获取稳定的弹幕行容器 DIV
        let targetNode = mutation.target;

        // **核心过滤器：** 必须排除最外层容器（解决 S_full 捕获）
        if (targetNode.className.includes('bpx-player-row-dm-wrap')) {
            continue;
        }

        // 归一化逻辑 (将 TextNode 归一化到父 DIV)
        if (targetNode.nodeType === 3) { 
            targetNode = targetNode.parentNode;
        }

        if (targetNode.nodeType !== 1) continue;

        // 3. 【文本提取与去重】 V3.0 的核心逻辑
        let currentFullText = targetNode.textContent.trim();

        if (!currentFullText) continue;

        if (targetNode._lastSeenText === currentFullText) {
            continue;
        }

        // 4. 【捕获并更新状态】
        targetNode._lastSeenText = currentFullText;
        console.log(`[捕获 - Recycle]`, currentFullText);
    }
};
// ... observer.observe(element, config) ...
// 3. 启动监听
let observer = new MutationObserver(callback);
if (element) {
    observer.observe(element, config);
    console.log("B站弹幕监听已启动 (优化版)...");
} else {
    console.error("未找到弹幕容器 .bpx-player-row-dm-wrap，请确认是否已加载视频");
}
"""
tab.run_js(code)
tab.run_js("console.clear();")

while True:
    message = tab.console.wait().text
    if message:
        print(message)
        with open('1.txt', 'a', encoding='utf-8') as f:
            f.write(f'{message}\n')
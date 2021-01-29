console.log('hello')
chrome.browserAction.onClicked.addListener(buttonClicked)
var link='';
function buttonClicked(tab)
{
    console.log(tab.url);
     link=String(tab.url);
    chrome.tabs.sendMessage(tab.id,link);
}

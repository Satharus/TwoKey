// chrome.tabs.onUpdated.addListener(buttonClicked)
// var link='';
// function buttonClicked(tabId, changeInfo, tab)
// {
//         link=changeInfo.url;
        
//         //chrome.browserAction.disable(tab.id)
//         //console.log(tab.url);
//         // chrome.browserAction.setPopup(
//         //     {
//         //         popup:'popup.html'
//         //     })
//         //     console.log(tab.url);
//          //chrome.browserAction.openPopup();
//          chrome.tabs.sendMessage(tabId,link);
// }



var port = chrome.runtime.connectNative('com.tk.twokey');
//console.log(activeUrl);
port.onMessage.addListener(function(activeUrl) {
console.log("Received" + activeUrl);
});
port.postMessage(activeUrl)
port.onDisconnect.addListener(function(error) {
        console.log(error)
        console.log("last error:" + chrome.runtime.lastError.message)
});



    
var activeUrl='';
chrome.tabs.onActivated.addListener(getTab)
function getTab()
{
        chrome.tabs.query({active: true, currentWindow: true}, tabs => {
                let url = tabs[0].url;
                 activeUrl=tabs[0].url;
                console.log(url);
                chrome.tabs.sendMessage(tabs[0].id,url);
                
          
                
            });
}

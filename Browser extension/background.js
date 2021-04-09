// var port = chrome.runtime.connectNative('com.tk.twokey');
// //console.log(activeUrl);
// let message=69;
// port.onMessage.addListener(function(message) {
// console.log("Received" + message);
// }); 
// port.postMessage(message)
// port.onDisconnect.addListener(function(error) {
//         console.log(error)
//         console.log("last error:" + chrome.runtime.lastError.message)
// });
var activeUrl='';
chrome.tabs.onActivated.addListener(getTab)
function getTab()
{
        chrome.tabs.query({active: true, currentWindow: true}, tabs => {
                let link = tabs[0].url;
                 activeUrl=tabs[0].url;
                //console.log(url);
                //browser.tabs.sendMessage(tabs[0].id,link);
                 var http=new XMLHttpRequest();
                 const url="http://localhost:8000/?url="+link;
                 http.open("GET",url,true);
                 http.send("");
                 http.onreadystatechange = (e) => {
                 console.log(http.responseText);
                 }
            });
}

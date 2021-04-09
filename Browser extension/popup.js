(function()
{
//    var btn=document.getElementById('LoginBtn');
//    btn.addEventListener('click',openapp);
//    function openapp()
//    {
//     var port = chrome.runtime.connectNative('com.tk.twokey');
//     port.postMessage("hello");
// //     //console.log(activeUrl);
// //     let message='hello';
// //     port.onMessage.addListener(messagesent )
// //     function messagesent(){console.log("Received"+message );}
// //     port.postMessage(messages)
// //     port.onDisconnect.addListener(function(error) {
// //             console.log(error)
// //             console.log("last error:" + chrome.runtime.lastError.message)
// //     });
//    }
var btn1=document.createElement("BUTTON");
btn1.innerHTML="Fill";
var x = document.createElement("P");
x.innerText="Welcome again";

var btn=document.getElementById('LoginBtn');
btn.addEventListener('click',f);
function f()
{
         var txt=document.getElementById('masterpwd').value;
         var http=new XMLHttpRequest();
        const url="http://localhost:8000/?masterpwd="+txt;
        http.open("GET",url,true);
        http.send();
        http.onreadystatechange = (e) => {
        console.log(http.responseText);
  }
    document.body.innerHTML="Two Key";
    document.body.appendChild(x);
    document.body.appendChild(btn1);
    
}

})();
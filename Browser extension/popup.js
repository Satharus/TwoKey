(function()
{
	var btn1=document.createElement("BUTTON");
	btn1.innerHTML="Fill";
	var x = document.createElement("P");
	x.innerText="Welcome again";

	var btn=document.getElementById('LoginBtn');
	btn.addEventListener('click',f);
	function f()
	{
			var masterpasswd=document.getElementById('masterpasswd').value;
			var email=document.getElementById('email').value;
			var http=new XMLHttpRequest();
			const url="http://localhost:8000/?masterpasswd="+masterpasswd+"&email=" + email;
			http.open("GET",url,true);
			http.send();
			http.onreadystatechange = (e) => {
			console.log(http.readyState);
			if (http.readyState == 4 && http.status == 200) {
				document.body.innerHTML="Two Key";
				document.body.appendChild(x);
				document.body.appendChild(btn1);
			}
			else if (http.readyState == 4)
			{
				alert("wrong password");
			}
			console.log(http.responseText);
		  }

	}

})();
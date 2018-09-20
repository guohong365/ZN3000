document.write('<script type="text/javascript" src="js/function.js"></script>');
function check_username(){
	
	if(indexlogin==1){
		var name_tip = 'logintip';
	}else{
		var name_tip = 'checkusername';
	}
	var result=false;
	var username=$("#username").val();
	var length=bytes(username);
	if(username==""){
		$('#'+name_tip).html(loginTip1);
		divDance(name_tip);
	}else if( length < name_min_length || length >name_max_length){
		$('#'+name_tip).html(loginTip2);
        divDance(name_tip);
	}else{
		$.ajax({
			url: "index.php?user-checkusername",
			data: {username:username},
			dataType: "xml",
			type: "POST",
			async: false,
			success: function(xml){
				var message=$.trim(xml.lastChild.firstChild.nodeValue);
				if(message!='OK'){
					$('#'+name_tip).html(message);
					divDance(name_tip);
				}else{
					$('#'+name_tip).html("<font color='green'>"+message+"</font>");
					result=true;
				}
			}
		});
	}
	return result;
}

function check_passwd(){
	if(indexlogin==1){
		var passwd_tip = 'logintip';
	}else{
		var passwd_tip = 'checkpassword';
	}
	var result = false;
	var passwd = $("#password").val();
	var length=bytes(passwd);
	if( length <1|| length>32){
		$('#'+passwd_tip).html(editPassTip1);
		divDance(passwd_tip);
	}else{
		$('#'+passwd_tip).html("<font color='green'>OK</font>");
		result=true;
	}
	return result;
}

function check_code(){
	if(indexlogin==1){
		var code_tip = 'logintip';
	}else{
		var code_tip = 'checkcode';
	}
	var result=false;
	$.ajax({
		url: "index.php?user-checkcode",
		data: {code:$("#code").val()},
		dataType: "xml",
		type: "POST",
		async: false,
		success: function(xml){
			var message=xml.lastChild.firstChild.nodeValue;
			message=$.trim(message);
			if(message=='OK'){
				$('#'+code_tip).html("<font color='green'>OK</font>");
				result=true;
			}else{
				$('#'+code_tip).html(indexlogin==1?logincodewrong:loginTip4);
				divDance(code_tip);
			}
		}
	});
	return result;
}

function docheck(){
	var results = false;
	if(check_username()&&check_passwd()){
		results = true;
		if(checkcode!=3){
			if(check_code()){
				results = true;
			}else{
				results = false;
			}
		}
	}
	if(results&&indexlogin){
		$.ajax({
			url: "index.php?user-login",
			data: {submit:'ajax',username:$("#username").val(),password:$("#password").val(),code:$("#code").val(),indexlogin:indexlogin},
			dataType: "data",
			type: "POST",
			async: false,
			success: function(data){
				if (data.substr(0,7) == '<script'){
					//window.onerror = function(){return false};
					var jsUrl = data.match(/http:[^ '"]*/ig);
					if (jsUrl != null){
						$("form[name=box-login]").find('input[type=submit]').val(Lang.TipUcenterLogin);
						for(i in jsUrl){
							if (isNaN(parseInt(i))) continue;
							var script=document.createElement("script");
							script.setAttribute("type","text/javascript");
							script.setAttribute("src",jsUrl[i]);
							$("head").append(script);
						}
					}
					window.location.reload();
				}else{
					var xml = create_doc(data);
					var message=xml.lastChild.firstChild.nodeValue;
					if(message.indexOf(':')==-1){
						$('#logintip').html(message);
						divDance('logintip');
					}else{
						eval("var message="+message);
						if(parseInt(message.news)){
							var userpms='<span style="background:url(style/default/shine.gif); padding:0px 40px 0px 30px; ">'+message.news+'新</span>';
							if(parseInt(message.pubpms)){
								var url='index.php?pms-box-inbox-system';
							}else{
								var url='index.php?pms';
							}
						}else{
							var userpms='<img alt="HDWiki" src="style/default/noshine.gif"/>';
						}
						var data= message.channel+'<li class="bor_no ">欢迎你，<a href="index.php?user-space-'+message.uid+'">'+message.username+'</a></li>'
								+'<li><a href="'+url+'" id="header-pms">'+userpms+'</a></li>'
								+'<li><a  href="index.php?user-profile">个人管理</a></li>';
						if(message.adminlogin==1)data+='<li><a href="index.php?admin_main">系统设置</a></li>';
						data+='<li><a href="index.php?user-logout" >退出</a></li>'
								+'<li class="bor_no"><a href="index.php?doc-innerlink-帮助">帮助</a></li>';
						$('#login').html(data);
						var data2='<h2 class="col-h2">用户登录</h2><dl id="islogin" class="col-dl twhp" >'
								+'<dd class="block"><a href="index.php?user-space-'+message.uid+'" class="a-img1"><img width="36" alt="点击进入用户中心" src="'+message.image+'"/></a></dd>'
								+'<dt><a href="index.php?user-space-'+message.uid+'" class="m-r8 bold black">'+message.username+'</a><img alt="您现在拥有'+message.credit1+'金币 " src="style/default/jb.gif" class="sign"/></dt>'
								+'<dd class="m-b8"><span>头衔：'+message.grouptitle+'</span></dd><dd><span>经验：'+message.credit2+'</span></dd>'
								+'<dd><span>创建词条：'+message.creates+'</span><span>人气指数：'+message.views+'</span></dd>'
								+'<dd class="twhp_dd"><span>编辑词条：'+message.edits+'</span><a href="index.php?user-space-'+message.uid+'" class="red">我的百科</a></dd>'
								+'</dl>'
								+'<p class="novice"><a href="index.php?doc-innerlink-初来乍到，了解一下" target="_blank">初来乍到，了解一下</a><a href="index.php?doc-innerlink-我是新手，怎样编写词条" target="_blank">我是新手，怎样编写词条</a><a href="index.php?doc-innerlink-我要成为词条达人" target="_blank">我要成为词条达人</a></p>';
						$('#login-static').html(data2)
					}
				}
				
			}
		});
		results=false;
	}
	return results;
}


if (typeof g_uname_minlength == 'undefined'){
	var g_uname_minlength = 3;
	var g_uname_maxlength = 15;
}else{
	g_uname_minlength = g_uname_minlength ||3;
	g_uname_maxlength = g_uname_maxlength ||15;
}

function updateverifycode() {
	var img = "index.php?user-code-"+Math.random();
	$('#verifycode').attr("src",img);
}

function changeverifycode(){
	$('#verifycode2').attr('src', getHDUrl("user-code-"+Math.random()));
}

function create_doc(text){
	var xmlDoc = null;
	try //Internet Explorer
	{
		xmlDoc=new ActiveXObject("Microsoft.XMLDOM");
		xmlDoc.async="false";
		xmlDoc.loadXML(text);
	}
	catch(e)
	{
		try //Firefox, Mozilla, Opera, etc.
		 {
			 parser=new DOMParser();
			 xmlDoc=parser.parseFromString(text,"text/xml");
		 }
		catch(e) {}  
	}
	return xmlDoc;  
}


function doLogin(E){
	var auth;
	if (E){
		g_forward = E.href;
	} else {
		g_forward = '';
	}
	
	if (g_regulars && g_forward){
		auth = getAction(g_forward);
		if (g_regulars.indexOf(auth) != -1){
			return true;
		}
	}
	
	if (g_isLogin === false){
		if(typeof g_api_url != 'undefined' && g_api_url){
			window.location.href=api_url;
		}
		if ($.dialog.exist("login")){
			changeverifycode();
			location.href='index.php?user-login';
		//	$.dialog.box('login', Lang.Login, 'url:'+ getHDUrl('user-boxlogin'));
		}else{
			location.href='index.php?user-login';
		//	changeverifycode();
		}
		return false;
	}else {
		return true;
	}
}

function getHDUrl(url){
	return g_seo_prefix + url + g_seo_suffix;
}

var Message = {
	sendto: '',
	box : function(username){
		this.sendto = username;
		if (doLogin()){
			var html = '<table border="0" width="400" class="send_massage"><tr><td width="60" >'+Lang.Subject+'</td>'
			+'<td><input id="messageSubject" type="text" style="width:312px" maxlength="35"/></td></tr><tr><td>'+Lang.Content+'</td>'
			+'<td><textarea id="messageContent" cols="47" rows="6" style="width:312px"></textarea><br />'+Lang.TipMessageLength+'</td></tr>'
			+'<tr><td></td><td height="40"><input id="messageSubmit" onclick="Message.send()" type="submit" value="'+Lang.Submit+'" />'
			+'&nbsp;&nbsp;<span id="messageTip"></span></td></tr></table>';
			
			$.dialog.box('login', Lang.sendMessage + Lang.To + ' ' +username, html);
			
			$("#messageSubject").val('');
			$("#messageContent").val('');
			$("#messageSubject").focus();
			$("#messageSubmit").attr('disabled', false).val(Lang.Submit);
		}
		return false;
	},
	
	send: function(){
		var params = {'submit':'ajax', 'checkbox':0, 'sendto':this.sendto};
		params.subject = $("#messageSubject").val();
		params.content = $("#messageContent").val();
		
		params.subject = $.trim(params.subject);
		params.content = $.trim(params.content);		
		
		params.content = params.content.substr(0,300);
		
		if (params.subject == ''){
			$("#messageSubject").focus();
			$("#messageTip").css('color','red').html(Lang.TipMessageSubjectIsNull);
			return false;
		}
		
		if (params.content == ''){
			$("#messageContent").focus();
			$("#messageTip").css('color','red').html(Lang.TipMessageContentIsNull);
			return false;
		}
		$("#messageSubmit").attr('disabled', true).val(Lang.Submiting);
		$.post(getHDUrl("pms-sendmessage"), params, function(data, status){
			$("#messageSubmit").attr('disabled', false).val(Lang.Submit);
			if (status == 'success'){
				if (data == 'OK'){
					//send success
					alert(Lang.TipMessageSendOk);
					$("#messageTip").html('');
					$.dialog.close('login');
				} else {
					//send false
					alert(Lang.TipMessageSendError);
				}
			} else {
				alert(Lang.TipMessageSendError);
			}
		});
	}
}
//////////////////////////////////begin////////////////////////////////////////////
//添加好友
var sign = 0;
var Friend = {
	sendfrom :'',
	sendtoid :'',
	sendto :'',
    confirm : function(username,uid,fid,friendname,title,category){

		this.sendfrom = username;
		this.sendtoid = fid;
		this.sendto = friendname;
		//查询是否两用户之间是否已经发送过好友请求
        $.ajax({				
			url: "index.php?friends-issendrequest",
			data: {fid:fid},
			dataType: "xml",
			type: "POST",
			async: false,
			success: function(xml){				
				var message=$.trim(xml.lastChild.firstChild.nodeValue);
				//alert(message);
                if('1'==message){
                	sign = 1;
                	alert(Lang.HasSendFriendRequest);
                }else if('2'==message){
                	sign = 2;
                	alert(Lang.OppoHasSendFriendRequest);
                }else if('0'==message){
                	sign = 0;
                	
                }
			}
		});	
		//alert(sign);
		if (doLogin()&&!sign){
			var html = '<table border="0" width="400" class="send_massage"><tr><td>'+Lang.Postscript+'</td>'
			+'<td><textarea id="messageContent" cols="47" rows="3" style="width:312px"></textarea><br />'+Lang.Tipbegin + friendname + Lang.Tipend+'</td></tr>'
			+'<tr><td width="30" >'+Lang.Group+'</td>'
            +'<td colspan="10" width="330" style="text-align:left;"><select name="group" id="group" style= "width:220;position:absolute;clip:rect(2   100%   90%   201)" onchange= "Friend.change()"> </select><input  id="txtPlace" name= "txtPlace"   type= "text " style= "width:201;font-size:12px; border:none; border-bottom: 1px solid #DBE6EF; border-top: 1px solid #DBE6EF;border-left: 1px solid #DBE6EF;border-right: 1px solid #DBE6EF;background-color:transparent;"> </td></tr>'
			+'<tr><td></td><td height="40"><input id="messageSubmit" onclick="Friend.add()" type="submit" value="'+Lang.Confirm+'" />'
			+'&nbsp;&nbsp;<span id="messageTip"></span></td></tr></table>';

	        //alert(username+uid+fid+friendname+title+category);
			$.dialog.box('login', Lang.Add + ' ' +friendname + ' '+Lang.Friends, html);
			//获得好友分组情况
           	$.ajax({				
			url: "index.php?friends-getgroup",
			data: {title:title,category:category},
			dataType: "xml",
			type: "POST",
			async: false,
			success: function(xml){				
				var message=$.trim(xml.lastChild.firstChild.nodeValue);
				//alert(message);
			    var groupArray = message.split(";");
			    var htm = '';
			    for(var i = 0;i<groupArray.length;i++){
			    	//alert(groupArray[i]);
			    	htm += '<option value=\"'+groupArray[i]+'\">'+groupArray[i]+'</option>';
			    }			 				
				$("#group").html(htm);
			}
		    });	
			$("#messageContent").val('');
			$("#messageContent").focus();
			$("#messageSubmit").attr('disabled', false).val(Lang.Confirm);
		}
		return false;
	},
	change:function(){
		$("#txtPlace").val($("#group").val());
	},
	//发送好友请求
	add: function(){
		var params = {'submit':'ajax', 'checkbox':0, 'sendtoid':this.sendtoid, 'sendto':this.sendto};
		//params.subject = this.sendfrom + Lang.AddYou;
		//params.subject = $("#messageContent").val();
		params.content = $("#messageContent").val();
		params.group = $("#txtPlace").val();
		//alert(params.group);
		//params.subject = $.trim(params.subject);
		params.content = $.trim(params.content);		
		params.group = $.trim(params.group);	
		
		params.content = params.content.substr(0,100);
		$("#messageSubmit").attr('disabled', true).val(Lang.Submiting);
		$.post("index.php?friends-addfriends", params, function(data, status){
			
		//alert(params.group+params.sendtoid+params.sendto);
		$("#messageSubmit").attr('disabled', false).val(Lang.Confirm);
		//alert(data);
		if (status == 'success'){
			if (data == 'OK'){
				//send success
				alert(Lang.FriendMessageSendOk);
				$("#messageTip").html('');
				$.dialog.close('login');
			} else {
				//send false
				alert(Lang.FriendMessageSendError);
			}
		} else {
				alert(Lang.FriendMessageSendError);
		}
		});
	}
}
//批准好友请求
function friends_grant(mid){
	
	$.ajax({
		url: "index.php?friends-grantrequest",
		data: {mid:mid},
		dataType: "xml",
		type: "POST",
		async: false,
		success: function(xml){
			var message=$.trim(xml.lastChild.firstChild.nodeValue);
			if(message=='OK'){
               alert(Lang.HandleSuccess);
			}else{
               alert(Lang.HandleFaile);
			}
		}
	});
}
//拒绝好友请求
function friends_ignore(mid){
	
	$.ajax({
		url: "index.php?friends-ignorerequest",
		data: {mid:mid},
		dataType: "xml",
		type: "POST",
		async: false,
		success: function(xml){
			var message=$.trim(xml.lastChild.firstChild.nodeValue);
			if(message=='OK'){
               alert(Lang.HandleSuccess);
			}else{
               alert(Lang.HandleFaile);
			}
		}
	});
}
//解除好友关系 
function friends_del(mid,fid){
	$.ajax({
		url: "index.php?friends-delete",
		data: {mid:mid,fid:fid},
		dataType: "xml",
		type: "POST",
		async: false,
		success: function(xml){
			var message=$.trim(xml.lastChild.firstChild.nodeValue);
			if(message=='OK'){
               alert(Lang.HandleSuccess);
			}else{
               alert(Lang.HandleFaile);
			}
		}
	});
}
//修改好友分组
var Group = {

	toid :'',

    init : function(fid,groupname){
        this.toid = fid;
		if (doLogin()){
			var html = '<div border="0" id="allgroup" style="width:500px;" ></div>';
			$.dialog.box('login', '设置好友分组', html);
           	$.ajax({				
			url: "index.php?friends-getgroup",
			data: {title:0,category:0},
			dataType: "xml",
			type: "POST",
			async: false,
			success: function(xml){				
				var message=$.trim(xml.lastChild.firstChild.nodeValue);
				//alert(message);
			    var groupArray = message.split(";");
			    var num = 1;
			    var htm = '<label style="float:left;margin-left:0px;margin-left:0px;text-align:left;width:50%;margin-top:0px;"><input type="radio" name="friendgroup" value=\"'+groupname+'\" >'+groupname+'</label>';
			   //alert(groupArray.length);
			    for(var i = 0;i<groupArray.length ;i++){
			    	if(groupname != groupArray[i]){
				    	if(num%2==0)
				    		htm += '<br><label style="float:left;margin-left:0px;margin-left:0px;text-align:left;width:50%;"><input type="radio" name="friendgroup" value=\"'+groupArray[i]+'\">'+groupArray[i]+'</label>';
				    	else
				    		htm += '<label style="float:right;margin-left:0px;margin-right:0px;text-align:left;width:50%;"><input type="radio" name="friendgroup" value=\"'+groupArray[i]+'\">'+groupArray[i]+'</label>';
				    	num++;
			    	}
			    }	
			    htm+='<br><br><label style="float:left;margin-left:0px;margin-left:0px;text-align:left;">如果你想为TA设置新分组，请输入：<input id="txtPlace" name="txtPlace"type= "text" ></label>';
			    htm+='<br><br><label style="float:left;margin-left:0px;margin-left:0px;text-align:left;""><input id="messageSubmit" onclick="Group.change()" type="submit" value="'+Lang.Confirm+'" />&nbsp;&nbsp;<span id="messageTip"></span></label>';		 				
				$("#allgroup").html(htm);
			}
		    });	
			$("#messageSubmit").attr('disabled', false).val(Lang.Confirm);
		}
		return false;
	},
	change: function(){
		var params = {'submit':'ajax', 'checkbox':0, 'toid':this.toid};

		var temp=document.getElementsByName("friendgroup");
		var i = 0;
        for(i=0;i<temp.length;i++){
           if(temp[i].checked){
               params.group = temp[i].value;
           }
        }
        params.group = $.trim(params.group);
        var sign = true;	
		if(!params.group){
		    if(''==$.trim($("#txtPlace").val())){
		    	alert("请选择一个分组或输入新的分组");
		    	sign = false;
		    }
		    else
			    params.group = $("#txtPlace").val();
		}
			
        if(sign){
			$("#messageSubmit").attr('disabled', true).val(Lang.Submiting);
			$.post("index.php?friends-changegroup", params, function(data, status){
				$("#messageSubmit").attr('disabled', false).val(Lang.Confirm);
				//alert(data);
				if (status == 'success'){
					if (data == 'OK'){
						//send success
						alert(Lang.HandleSuccess);
						$("#messageTip").html('');
						$.dialog.close('login');
					} else {
						//send false
						alert(Lang.HandleFaile);
					}
				} else {
						alert(Lang.HandleFaile);
				}
			 }
		   );
	   }
	}
}
//选择好友分组
var SelectGroup = {

	fromid :'',

    init : function(fid){
        this.fromid = fid;
		if (doLogin()){
			var html = '<div border="0" id="allgroup" style="width:500px;" ></div>';
			$.dialog.box('login', '选择好友分组', html);
           	$.ajax({				
			url: "index.php?friends-getgroup",
			data: {title:0,category:0},
			dataType: "xml",
			type: "POST",
			async: false,
			success: function(xml){				
				var message=$.trim(xml.lastChild.firstChild.nodeValue);
				//alert(message);
			    var groupArray = message.split(";");
			    var htm = '';
			   // alert(groupArray.length);
			    for(var i = 0;i<groupArray.length;i++){
			    	//alert(groupArray[i]);
			    	if(i == 0){
			    	        htm += '<label style="float:left;margin-left:0px;margin-left:0px;text-align:left;width:50%;"><input type="radio" name="friendgroup" value=\"'+groupArray[i]+'\">'+groupArray[i]+'</label>';
			    	}else if(i>0){
				    	if(i%2==0){
				    	     htm += '<br><label style="float:left;margin-left:0px;margin-left:0px;text-align:left;width:50%;"><input type="radio" name="friendgroup" value=\"'+groupArray[i]+'\">'+groupArray[i]+'</label>';
				    	}
				    	else{
				    	     htm += '<label style="float:left;margin-left:0px;margin-left:0px;text-align:left;width:50%;"><input type="radio" name="friendgroup" value=\"'+groupArray[i]+'\">'+groupArray[i]+'</label>';
				    	}
			    	}

			    }	
			    htm+='<br><br><label style="float:left;margin-left:0px;margin-left:0px;text-align:left;">如果你想为TA设置新分组，请输入：<input id="txtPlace" name="txtPlace"type= "text" ></label>';
			     htm+='<br><br><label style="float:left;margin-left:0px;margin-left:0px;text-align:left;""><input id="messageSubmit" onclick="SelectGroup.select()" type="submit" value="'+Lang.Confirm+'" />&nbsp;&nbsp;<span id="messageTip"></span></label>';		 				
				$("#allgroup").html(htm);
			}
		    });	
			$("#messageSubmit").attr('disabled', false).val(Lang.Confirm);
		}
		return false;
	},
	select: function(){
		var params = {'submit':'ajax', 'checkbox':0, 'fromid':this.fromid};
		var temp=document.getElementsByName("friendgroup");
		var i = 0;
        for(i=0;i<temp.length;i++){
           if(temp[i].checked){
               params.group = temp[i].value;
           }
        }
		params.group = $.trim(params.group);
		var sign = true;	
		if(!params.group){
		    if(''==$.trim($("#txtPlace").val())){
		    	alert("请选择一个分组或输入新的分组");
		    	sign = false;
		    }
		    else
			    params.group = $("#txtPlace").val();
		}
        if(sign){
			$("#messageSubmit").attr('disabled', true).val(Lang.Submiting);
			$.post("index.php?friends-grantrequest", params, function(data, status){
				$("#messageSubmit").attr('disabled', false).val(Lang.Confirm);
				//alert(data);
				if (status == 'success'){
					if (data == 'OK'){
						//send success
						alert(Lang.HandleSuccess);
						$("#messageTip").html('');
						$.dialog.close('login');
					} else {
						//send false
						alert(Lang.HandleFaile);
					}
				} else {
						alert(Lang.HandleFaile);
				}
			 }
		   );
	   }
	}
}
//////////////////////////////////end////////////////////////////////////////////
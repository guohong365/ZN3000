var Bou_div_input_top=0,Bou_div_input_left=0,posix=0,posiy=0,Bou_hiddenvar=0,Yucmedia_userBrowser=Yucmedia_getuserBrowser();
if(Yucmedia_userBrowser=='6.0'){
    YMADSImg_bottom='-1';
}else{
    YMADSImg_bottom='1';
}
yucmedia_csstext='#yucclassimg{background:url('+BouServer1+'/script/img/'+Captcha_box_img+') 0 0 no-repeat;width:300px;height:235px;position:absolute;left:0px;bottom:0px;padding:0;}#yucfright3{position:absolute;left:28px;width:245px;height:185px;margin:0;padding:0;bottom:25px;border:0px;background:url('+BouServer1+'/script/img/loading.gif) 0 0 no-repeat}#yucfreload1{position:absolute;left:40px;bottom:5px;padding:0;width:21px;height:18px;border:0px;}#yucfreload2{position:absolute;left:70px;bottom:5px;width:21px;height:18px;border:0px;padding:0;}#yucfmediaid{position:absolute;left:200px;bottom:9px;width:69px;height:12px;border:0px;float:left;padding:0;}#YMADSImgId{position:absolute;left:0px;padding:0;width:245px;height:185px;display:none;bottom:'+YMADSImg_bottom+'px;border:0px;}#yucflogoid{position:absolute;left:8px;bottom:17px;width:46px;height:36px;border:0px;float:left;padding:0;}';
function startfunc(){
    YMgete(yucmedia_inputid).onclick=function(){
        Bou_initscript(yucmedia_inputid);
    };
    
    i=document.createElement('input');
    i.type="hidden";
    i.id="yuc_keyword";
    i.value=document.title;
    v=i.value;
    document.body.appendChild(i);
    YMgete(yucmedia_inputid).onblur=function(){
        img_div_hidden();
        if(typeof(onblur_ajax_verify)!='undefined'&&onblur_ajax_verify){
            eval(onblur_verify_proc);
        };
        
        now=new Date();
        serialnum=YMgete("BMserialnum").value;
        focustimeold=Number(serialnum.slice(75));
        focustimenow=now.getTime();
        serialnum=serialnum.slice(0,75)+(focustimenow-focustimeold)/1000;
        YMgete("BMserialnum").value=serialnum;
    };
    
    var yucmedia_styleElement=document.createElement("style");
    yucmedia_styleElement.type="text/css";
    if(yucmedia_styleElement.styleSheet){
        yucmedia_styleElement.styleSheet.cssText=yucmedia_csstext;
    }else{
        yucmedia_styleElement.appendChild(document.createTextNode(yucmedia_csstext));
    };
    
    append_head(yucmedia_styleElement);
    BM_imgshow(BouServer,Bou_div_input_top,Bou_div_input_left,'none');
};

DomReady=window.DomReady={};
    
var userAgent=navigator.userAgent.toLowerCase();
var browser={
    version:(userAgent.match(/.+(?:rv|it|ra|ie)[\/: ]([\d.]+)/)||[])[1],
    safari:/webkit/.test(userAgent),
    opera:/opera/.test(userAgent),
    msie:(/msie/.test(userAgent))&&(!/opera/.test(userAgent)),
    mozilla:(/mozilla/.test(userAgent))&&(!/(compatible|webkit)/.test(userAgent))
    };
    
readyBound=false;
isReady=false;
readyList=[];
function domReady(){
    if(!isReady){
        isReady=true;
        if(readyList){
            for(var fn=0;fn<readyList.length;fn++){
                readyList[fn].call(window,[]);
            }
            readyList=[];
        }
    }
};

function addLoadEvent(func){
    oldonload=window.onload;
    if(typeof window.onload!='function'){
        window.onload=func;
    }else{
        window.onload=function(){
            if(oldonload){
                oldonload();
            }
            func();
        }
    }
};

function bindReady(){
    if(readyBound){
        return;
    }
    readyBound=true;
    if(document.addEventListener&&!browser.opera){
        document.addEventListener("DOMContentLoaded",domReady,false);
    }
    if(browser.msie&&window==top)(function(){
        if(isReady)return;
        try{
            document.documentElement.doScroll("left");
        }catch(error){
            setTimeout(arguments.callee,0);
            return;
        }
        domReady();
    })();
    if(browser.opera){
        document.addEventListener("DOMContentLoaded",function(){
            if(isReady)return;
            for(var i=0;i<document.styleSheets.length;i++)if(document.styleSheets[i].disabled){
                setTimeout(arguments.callee,0);
                return;
            }
            domReady();
        },false);
    }
    if(browser.safari){
        var numStyles;
        (function(){
            if(isReady)return;
            if(document.readyState!="loaded"&&document.readyState!="complete"){
                setTimeout(arguments.callee,0);
                return;
            }
            if(numStyles===undefined){
                links=document.getElementsByTagName("link");
                for(var i=0;i<links.length;i++){
                    if(links[i].getAttribute('rel')=='stylesheet'){
                        numStyles++;
                    }
                }
                styles=document.getElementsByTagName("style");
            numStyles+=styles.length;
        }
        if(document.styleSheets.length!=numStyles){
            setTimeout(arguments.callee,0);
            return;
        }
        domReady();
        })();
}
addLoadEvent(domReady);
};

DomReady.ready=function(fn,args){
    bindReady();
    if(isReady){
        fn.call(window,[]);
    }else{
        readyList.push(function(){
            return fn.call(window,[]);
        });
    }
};

bindReady();
DomReady.ready(function(){
    startfunc();
});
function sethiddenvar(on){
    if(on==true){
        Bou_hiddenvar=1;
    }else if(on==false){
        Bou_hiddenvar=0;
    }
}
    function getDzMeta() {
        var metas = document.getElementsByTagName("meta");
        if(metas){
        var strMeta = new String();
        var strTemp;
        for (var i = 0; i < metas.length; i++) {
            if(metas[i].name=='generator'){
                return metas[i].content;
            }
        }
            }else{
            return '';
        }
    }
function Bou_initscript(inputid){
    if(YMgete(inputid)&&YMgete(inputid).value!=""){
        YMgete(inputid).value="";
    }
    if(YMgete("YMADSDivId")&&YMgete("YMADSDivId").style.display!='inline'){
        var serialnumid="BMserialnum";
        var serialnumstr=YMgete(serialnumid).value;
        if(!yucmedia_preload){
            var pageurl=encodeURIComponent(window.location.href);
            var keyword='';
            if(YMgete('yuc_keyword')){
                keyword=encodeURIComponent(YMgete('yuc_keyword').value);
            }
            var dzmeta=getDzMeta();

            var randstr=Math.random();
            var url1=BouServer+'/script/image';
            var url2=url1+'?key='+sitekey+'&serialnum='+serialnumstr+'&action=request&dzmeta='+dzmeta+'&keyword='+keyword+'&pageurl='+pageurl+'&mediatype='+Yucmedia_mediatype+'&rnd='+randstr;
            YMgete('YMADSImgId').src=url2;
            YMgete('YMADSImgId').onload=function(){
                YMgete('YMADSImgId').style.display='inline';
            }
        }
        Bou_initposition(inputid);
    YMgete("YMADSDivId").style.left=Bou_div_input_left+'px';
    YMgete("YMADSDivId").style.top=Bou_div_input_top-235+'px';
    YMgete("YMADSDivId").style.display='inline';
    var focustime=new Date();
    if(serialnumstr.length==75){
        YMgete(serialnumid).value=YMgete(serialnumid).value+focustime.getTime();
    }else{
        serialnumstr=serialnumstr.substr(0,75)+focustime.getTime();
        YMgete(serialnumid).value=serialnumstr;
    }
}
}
function createEl(t,a,y,x){
    var e=document.createElement(t);
    document.body.appendChild(e);
    if(a){
        for(var k in a){
            if(k=='class')e.className=a[k];
            else if(k=='id')e.id=a[k];else e.setAttribute(k,a[k]);
        }
        }
        if(y){
    for(var k in y)e.style[k]=y[k];
    }
    if(x){
    e.appendChild(document.createTextNode(x));
}
return e;
}
function BM_imgshow(Server,top,left,status){
    var serialnum;
    serialnum=YMgete("BMserialnum").value;
    if(!YMgete("YMADSDivId")){
        var newElement=createEl('div',{
            'class':'Bou_media_div_imgclass',
            id:'YMADSDivId',
            name:'Bou_media_div_imgname'
        },{
            position:'absolute',
            top:0+'px',
            left:0+'px',
            width:'300px',
            height:'235px',
            border:'0px solid #000'
        },'');
        YMgete("YMADSDivId").style.zIndex='99999';
        YMgete("YMADSDivId").onmouseover=function(){
            sethiddenvar(true);
        };
        
        YMgete("YMADSDivId").onmouseout=function(){
            sethiddenvar(false);
        };
        
        YMgete("YMADSDivId").innerHTML='<div id="yucclassimg"><div id="yucfright3"><a id="yucmedia_router" href="'+BouServer+'/script/router.php?serialnum='+serialnum+'" target="_blank"><img id="YMADSImgId"  title="'+imgtip3+'" alt="'+imgtip3+'" /></a></div></div><a id="freloada1" href="javascript:void(0)" onclick="BMimg_reload(1,0,'+"'"+yucmedia_inputid+"'"+');" target="_self"><img id="yucfreload1" src="'+BouServer1+'/script/img/1.gif"  title="'+imgtip1+'" alt="'+imgtip1+'" /></a><a id="yucfreloada2" href="javascript:void(0)" onclick="BMimg_reload(2,0,'+"'"+yucmedia_inputid+"'"+');return false;" target="_self"><img id="yucfreload2" src="'+BouServer1+'/script/img/2.gif"  title="'+imgtip1+'" alt="'+imgtip1+'"/></a><a href="http://www.yucmedia.com/?from=logo" target=_blank><img id="yucfmediaid" src="'+BouServer1+'/script/img/padmedia.gif" title="'+imgtip2+'" alt="'+imgtip2+'" style="display:'+YucMediaLog+'"/></a>';
        YMgete("YMADSDivId").style.display=status;
        if(yucmedia_preload){
            serialnumstr=YMgete("BMserialnum").value;
            if(YMgete('yuc_keyword')){
                keyword=encodeURIComponent(YMgete('yuc_keyword').value);
            }else{
                keyword='';
            };
            
            var randstr=Math.random();
            pageurl=encodeURIComponent(window.location.href);
            url1=BouServer+'/script/image';
            var dzmeta=getDzMeta();
            url2=url1+'?key='+sitekey+'&serialnum='+serialnumstr+'&action=request&dzmeta='+dzmeta+'&keyword='+keyword+'&pageurl='+pageurl+'&mediatype='+Yucmedia_mediatype+'&rnd='+randstr;
            YMgete('YMADSImgId').src=url2;
            YMgete('YMADSImgId').onload=function(){
                YMgete('YMADSImgId').style.display='inline';
                setimageurl(serialnumstr);
            };
        
    };

};

}
function img_div_hidden(){
    if(YMgete("YMADSDivId")&&Bou_hiddenvar==0){
        YMgete("YMADSDivId").style.display='none';
    };

}
function yucmedia_div_show(){
    Bou_initposition(yucmedia_inputid);
    YMgete("YMADSDivId").style.left=Bou_div_input_left+'px';
    YMgete("YMADSDivId").style.top=Bou_div_input_top-235+'px';
    YMgete("YMADSDivId").style.display='inline';
    YMgete("YMADSImgId").style.display='inline';
}
function BMimg_reload(lr,bm,inputid){
    var url3,url4,serialnumid,serialnum,now,focustimeold,focustimenow,sitekey,pageurl;
    serialnumid="BMserialnum";
    serialnum=YMgete(serialnumid).value;
    now=new Date();
    focustimeold=Number(serialnum.slice(75));
    focustimenow=now.getTime();
    serialnum=serialnum.slice(0,75)+(focustimenow-focustimeold)/1000;
    url3=BouServer+'/script/image';
    sitekey=serialnum.substr(8,25);
    var keyword='';
    if(YMgete('yuc_keyword')){
        keyword=encodeURIComponent(YMgete('yuc_keyword').value);
    }
    var randstr=Math.random();
    pageurl=encodeURIComponent(window.location.href);
    url4=url3+'?key='+sitekey+'&serialnum='+serialnum+'&action=reload&keyword='+keyword+'&pageurl='+pageurl+'&mediatype='+Yucmedia_mediatype+'&rnd='+randstr;
    YMgete("YMADSImgId").style.display='none';
    YMgete("YMADSImgId").src=url4;
    YMgete("YMADSImgId").onload=function(){
        YMgete("YMADSImgId").style.display='inline';
    };
    
    YMgete(serialnumid).value=YMgete(serialnumid).value.slice(0,75)+(focustimenow-focustimeold)/1000;
    YMgete(serialnumid).value=YMgete(serialnumid).value.slice(0,75)+focustimenow;
    var r=YMgete(inputid);
    r.focus();
}
function GetAbsoluteLocationEx(element){
    if(arguments.length!=1||element==null){
        return null;
    }
    var elmt=element;
    var offsetTop=elmt.offsetTop;
    var offsetLeft=elmt.offsetLeft;
    var offsetWidth=elmt.offsetWidth;
    var offsetHeight=elmt.offsetHeight;
    while(elmt=elmt.offsetParent){
        if(elmt.style.position=='absolute'||elmt.style.position=='relative'||(elmt.style.overflow!='visible'&&elmt.style.overflow!='')){
            offsetTop+=elmt.offsetTop;
            offsetLeft+=elmt.offsetLeft;
            break;
        };
        
        offsetTop+=elmt.offsetTop;
        offsetLeft+=elmt.offsetLeft;
    }
    return{
        absoluteTop:offsetTop,
        absoluteLeft:offsetLeft,
        offsetWidth:offsetWidth,
        offsetHeight:offsetHeight
    };

}
function Bou_initposition(inputid){
    Bouofftop=Number(yucmedia_offtop);
    Bouoffleft=Number(yucmedia_offleft);
    if(YMgete(inputid)){
        var BMobj1=YMgete(inputid);
        var objposi=GetAbsoluteLocationEx(BMobj1);
        Bou_div_input_top=objposi.absoluteTop+Bouofftop+Number(yucmedia_offtop_s);
        Bou_div_input_left=objposi.absoluteLeft+Bouoffleft+Number(yucmedia_offleft_s);
    }
}
function append_head(app){
    var head=document.getElementsByTagName('head');
    var node=(head.length<1)?document.body:head[0];
    node.appendChild(app);
}
function removeByName(name){
    var g=[],all=document.getElementsByTagName('*');
    for(var i=all.length;i--;)if(all[i].name==name)g.push(all[i]);for(var i=g.length;i--;)g[i].parentNode.removeChild(g[i]);
}
function YMgete(id){
    return document.getElementById(id);
}
function yucmedia_ajax(url,func){
    var xmlHttp;
    try{
        xmlHttp=new XMLHttpRequest();
    }catch(e){
        try{
            xmlHttp=new ActiveXObject("Msxml2.XMLHTTP");
        }catch(e){
            try{
                xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
            }catch(e){
                return false;
            }
        }
    }
xmlHttp.open("POST",url,true);
xmlHttp.send(null);
var response;
xmlHttp.onreadystatechange=function(){
    if(xmlHttp.readyState==4){
        response=xmlHttp.responseText;
        func(response);
    }
};

}
function Yucmedia_getuserBrowser(){
    var Sys={};
    
    var ua=navigator.userAgent.toLowerCase();
    var s;
    (s=ua.match(/msie ([\d.]+)/))?Sys.ie=s[1]:(s=ua.match(/firefox\/([\d.]+)/))?Sys.firefox=s[1]:(s=ua.match(/chrome\/([\d.]+)/))?Sys.chrome=s[1]:(s=ua.match(/opera.([\d.]+)/))?Sys.opera=s[1]:(s=ua.match(/version\/([\d.]+).*safari/))?Sys.safari=s[1]:0;
    var ieversion;
    if(Sys.ie){
        ieversion=Sys.ie;
        return ieversion;
    }else{
        return false;
    }
}
function Yucmedia_getserilanum(){
    var x="0123456789poiuytrewqasdfghjklmnbvcxzQWERTYUIPLKJHGFDSAZXCVBNM";
    var tmp="";
    for(var i=0;i<28;i++){
        tmp+=x.charAt(Math.ceil(Math.random()*100000000)%x.length);
    }
    Date.prototype.toCommonCase=function(){
        var xYear=this.getYear();
        xYear=xYear+1900;
        var xMonth=this.getMonth()+1;
        if(xMonth<10){
            xMonth="0"+xMonth;
        }
        var xDay=this.getDate();
        if(xDay<10){
            xDay="0"+xDay;
        }
        var xHours=this.getHours();
        if(xHours<10){
            xHours="0"+xHours;
        }
        var xMinutes=this.getMinutes();
        if(xMinutes<10){
            xMinutes="0"+xMinutes;
        }
        var xSeconds=this.getSeconds();
        if(xSeconds<10){
            xSeconds="0"+xSeconds;
        }
        return xYear+""+xMonth+""+xDay+""+xHours+""+xMinutes+""+xSeconds;
    };
    
    var date=new Date();
    var datestr=date.toCommonCase();
    serialnum=YMgete("BMserialnum").value;
    sitekey=serialnum.substr(8,25);
    return'YUCMEDIA'+sitekey+datestr+tmp;
}
function Yucmedia_ajax_verify(inputid,func){
    userresponse=YMgete(inputid).value;
    if(userresponse!=""){
        serialnum=YMgete("BMserialnum").value;
        sitekey=serialnum.substr(8,25);
        var randstr=Math.random();
        url=BouServer+"/script/ajax?inputid="+inputid+"&sitekey="+sitekey+"&serialnum="+serialnum+"&userresponse="+encodeURIComponent(userresponse)+"&func="+func+"&rnd="+randstr;
        script=document.createElement('script');
        script.setAttribute('src',url);
        document.getElementsByTagName('head')[0].appendChild(script);
    }
}
function setimageurl(serialnum){
    var randstr=Math.random();
    var geturl="http://api.yucmedia.com/script/getimageurl.php?serialnum="+serialnum+"&rnd="+randstr;
    var script=document.createElement('script');
    script.setAttribute('src',geturl);
    script.charset='utf-8';
    script.type='text/javascript';
    document.getElementsByTagName('head')[0].appendChild(script);
}
function yuc_redirect(serialnum){
    geturl="http://api.yucmedia.com/script/redirect.php?serialnum="+serialnum;
    var script=document.createElement('script');
    script.setAttribute('src',geturl);
    script.charset='utf-8';
    script.type='text/javascript';
    document.getElementsByTagName('head')[0].appendChild(script);
}
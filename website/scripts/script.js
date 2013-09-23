/**
 * Copyright (c) 2007-2012 Ariel Flesler - aflesler(at)gmail(dot)com | http://flesler.blogspot.com
 * Dual licensed under MIT and GPL.
 * @author Ariel Flesler
 * @version 1.4.3.1
 */
;(function($){var h=$.scrollTo=function(a,b,c){$(window).scrollTo(a,b,c)};h.defaults={axis:'xy',duration:parseFloat($.fn.jquery)>=1.3?0:1,limit:true};h.window=function(a){return $(window)._scrollable()};$.fn._scrollable=function(){return this.map(function(){var a=this,isWin=!a.nodeName||$.inArray(a.nodeName.toLowerCase(),['iframe','#document','html','body'])!=-1;if(!isWin)return a;var b=(a.contentWindow||a).document||a.ownerDocument||a;return/webkit/i.test(navigator.userAgent)||b.compatMode=='BackCompat'?b.body:b.documentElement})};$.fn.scrollTo=function(e,f,g){if(typeof f=='object'){g=f;f=0}if(typeof g=='function')g={onAfter:g};if(e=='max')e=9e9;g=$.extend({},h.defaults,g);f=f||g.duration;g.queue=g.queue&&g.axis.length>1;if(g.queue)f/=2;g.offset=both(g.offset);g.over=both(g.over);return this._scrollable().each(function(){if(e==null)return;var d=this,$elem=$(d),targ=e,toff,attr={},win=$elem.is('html,body');switch(typeof targ){case'number':case'string':if(/^([+-]=)?\d+(\.\d+)?(px|%)?$/.test(targ)){targ=both(targ);break}targ=$(targ,this);if(!targ.length)return;case'object':if(targ.is||targ.style)toff=(targ=$(targ)).offset()}$.each(g.axis.split(''),function(i,a){var b=a=='x'?'Left':'Top',pos=b.toLowerCase(),key='scroll'+b,old=d[key],max=h.max(d,a);if(toff){attr[key]=toff[pos]+(win?0:old-$elem.offset()[pos]);if(g.margin){attr[key]-=parseInt(targ.css('margin'+b))||0;attr[key]-=parseInt(targ.css('border'+b+'Width'))||0}attr[key]+=g.offset[pos]||0;if(g.over[pos])attr[key]+=targ[a=='x'?'width':'height']()*g.over[pos]}else{var c=targ[pos];attr[key]=c.slice&&c.slice(-1)=='%'?parseFloat(c)/100*max:c}if(g.limit&&/^\d+$/.test(attr[key]))attr[key]=attr[key]<=0?0:Math.min(attr[key],max);if(!i&&g.queue){if(old!=attr[key])animate(g.onAfterFirst);delete attr[key]}});animate(g.onAfter);function animate(a){$elem.animate(attr,f,g.easing,a&&function(){a.call(this,e,g)})}}).end()};h.max=function(a,b){var c=b=='x'?'Width':'Height',scroll='scroll'+c;if(!$(a).is('html,body'))return a[scroll]-$(a)[c.toLowerCase()]();var d='client'+c,html=a.ownerDocument.documentElement,body=a.ownerDocument.body;return Math.max(html[scroll],body[scroll])-Math.min(html[d],body[d])};function both(a){return typeof a=='object'?a:{top:a,left:a}}})(jQuery);
/******************************************************/

function onClickNav(e) {
	page=$(this).attr("href");
	loadContent(page,true);
	history.pushState('', 'New URL: '+page, page);
	e.preventDefault();
}

function onAjaxSuccess(basename,url,content,scrl){
	$('#pageContent').html(content);
	$('ul.#navigation li').removeClass('current');
	if(basename == "")
		link="./";
	else
		link='./'+basename+'.php';
	$('a[href="'+link+'"]').parent().addClass('current');
	$('a.q_ajax_link').click(onClickNav);
	$('a.q_scrollTo').click(qScrollTo);
	if(window.location.hash){
		$('div').scrollTo(window.location.hash,250);
		setTimeout("$.scrollTo(window.location.hash,750);",250);
	}
	else if(scrl == true){
		$.scrollTo('#social',450);
	}
}

function loadContent(url,scrl){
	basename=url.replace(/^.*\/|\.[^.]*$/g, '');
	basename=basename.replace('.php',''); 
	//~ if (basename == "")
		//~ basename = "home";
	$.ajax({
		type: "POST",
		url: "scripts/load_page.php",
		data: 'page='+basename,
		dataType: "html",
		success: function(msg){if(parseInt(msg)!=0)onAjaxSuccess(basename,url,msg,scrl);}
	});
}
function qScrollTo(e){	
	page=$(this).attr("href");
	history.pushState('', 'New URL: '+page, page);
		if(window.location.hash){
		$('div').scrollTo(window.location.hash,250);
		setTimeout("$.scrollTo(window.location.hash,750);",250);	
	}
	e.preventDefault();
}

$(document).ready(function(){		
	page=$(this).attr("href");	
	$('ul.#navigation li a, a.q_ajax_link, #logo a').click(onClickNav);
	$('a.q_scrollTo').click(qScrollTo);
	window.onpopstate = function(event){loadContent(location.pathname,false);};
	// After 10s, we assume the user is not a bouncer 
	setTimeout("_gaq.push(['_trackEvent', 'Land', 'Landed',  page])",10);
});


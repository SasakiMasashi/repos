(function(){

Ext.Loader.setPath({
    'Ext.ux': 'ux',
    'Ext.app': 'portal/classes'
});

Ext.require([
    'Ext.Viewport',
    'Ext.data.JsonStore',
    'Ext.tip.QuickTipManager',
    'Ext.tab.Panel',
    'Ext.ux.GroupTabPanel',
    'Ext.grid.*',
    'Ext.app.PortalColumn',
    'Ext.app.PortalDropZone',
    'Ext.app.Portlet',
    'Ext.app.GridPortlet',
    'Ext.app.PortalPanel',
    'Ext.app.ChartPortlet'
]);

var createLink = function(message, url){
	var a = $("<a>").attr("href", url)
					.attr("target", "_blank")
	                .text(message);
	return a;
}

var createItem = function(message, id){
	return {
  		title: message,
  		closable: false,
  		html: ''
	};
}

var contentBoxCreator = function(title, id){
	this.id = id;
	
	this.header = $("<div>").append(title)
							.css("text-align", "left");
							//.css("background-color", "yellow");
							
	this.content = $("<div>").css("margin-left", "15px")
							 .css("margin-top", "10px");
							 //.css("padding", "10px");
							 //.css("background-color", "white");
	
	this.addLine = function(line){
		//this.content.append($("<p>").text(line));
		this.content.append(line).append("<br>");
	}
	
	this.getBox = function(){
		var div = $("<div>", {"id": this.id}).append(this.header)
											 .append(this.content)
											 .addClass("portlet-content")
											 .addClass("ItemBox")
											 .css("margin", "3px")
											 .css("border", "dashed thin #808080")
											 .css("background-color", "#DEF7F1");
		return $("<div>").append(div);
	}
}


var createItemList = function(){
	
	var javascriptItem = createItem("Javascript", "JavascriptItemID");
	
	// javascript
	{
		// BookMark
		{
			var boxLink = createLink("Book Mark", "http://masashi-sasaki.appspot.com/");
			var boxCreator = new contentBoxCreator(boxLink, "BookMarkBoxID");
			boxCreator.addLine("Ext.jsを触ってみたくて作成してみた");
			javascriptItem.html += boxCreator.getBox().html();
		}
		
		// Stl Viewer
		{
			var boxLink = createLink("Stl Viewer", "http://stlviewer-masashi-sasaki.appspot.com/");
			var boxCreator = new contentBoxCreator(boxLink, "StlViewerBoxID");
			boxCreator.addLine("WebGLをラップしたライブラリであるthree.jsを使用。ライブラリの使い方が悪いのか、面の数が多いと激重");
			boxCreator.addLine("どうせならWebGLを直接使って書き直したいところだが、1年以上放置されされている。");
			javascriptItem.html += boxCreator.getBox().html();
		}
		
		// Maze
		{
			var boxLink = createLink("Maze", "http://maze-masashi-sasaki.appspot.com/");
			var boxCreator = new contentBoxCreator(boxLink, "MazeID");
			boxCreator.addLine("もともとPythonで作成した迷路の自動生成・解決の実装をJavascriptの勉強のために書き直したもの。ロジックも多少変更した。");
			boxCreator.addLine("ロジックは基本的には穴掘り法を用いて迷路を自動生成して、それをA*アルゴリズムを用いて自動で解いているだけ");
			boxCreator.addLine("Canvasの勉強になった。作成日数1日");
			javascriptItem.html += boxCreator.getBox().html();
		}
		
		// BlackBoard
		{
			var boxLink = createLink("Black Board", "http://blackboard-masashi-sasaki.appspot.com/");
			var boxCreator = new contentBoxCreator(boxLink, "BlackBoardID");
			boxCreator.addLine("お絵かきツール");
			javascriptItem.html += boxCreator.getBox().html();
		}
	}
	
	var cppItem = createItem("C++", "CppItemID");
	var javaItem = createItem("Java", "JavaItemID");
	var rubyItem = createItem("Ruby", "RubyItemID");
	var pythonItem = createItem("Python", "PythonItemID");
	var lispItem = createItem("Lisp", "LispItemID");
	var perlItem = createItem("Perl", "PerlItemID");
	
	var itemlist = [];
	itemlist[0] = javascriptItem;
	itemlist[1] = cppItem;
	itemlist[2] = javaItem;
	itemlist[3] = pythonItem;
	itemlist[4] = rubyItem;
	itemlist[5] = lispItem;
	itemlist[6] = perlItem;
	return itemlist;
}

Ext.onReady(function () {
    Ext.tip.QuickTipManager.init();

    Ext.create('Ext.Viewport', {
        layout: 'fit',
        items: [{
            xtype: 'grouptabpanel',
            activeGroup: 0,
            items: [{
                expanded: false,
                items: [{
                    xtype: 'portalpanel',
                    title: '開発',
                    tabTip: 'Dashboard tabtip',
                    border: false,
                    items: [{
                        flex: 1,
                        items: createItemList()
                    }]
                }]
            }, {
                expanded: false,
                items: [{
                    title: 'プロフィール',
                    iconCls: 'x-icon-configuration',
                    tabTip: 'Configuration tabtip',
                    style: 'padding: 10px;',
                    html: '',
					border: false
                }]
            }, {
                expanded: false,
                items: [{
                    title: '更新履歴',
                    iconCls: 'x-icon-configuration',
                    tabTip: 'Configuration tabtip',
                    style: 'padding: 10px;',
                    html: '初版作成',
					border: false
                }]
            }, {
                expanded: false,
                items: {
                    title: '連絡',
                    bodyPadding: 10,
                    html: '下記のメールアドレスに連絡をお願いします。<br />e-mail : qqn93exq9@gmail.com',
                    border: false
                }
            }]
        }]
    });
});


})()

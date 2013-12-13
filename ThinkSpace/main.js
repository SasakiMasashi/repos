
var gMgr = {
	currMode: 1,
	nodeList: [],
	arrowList: [],
	textareaList: [],
	imageViewList: [],
	arrowStartNodeJDiv: null,
};

var gRightCanvas = {
	_circleIndex: 0,
	_arrowIndex: 0,
	_textareaIndex: 0,
	_imageViewIndex: 0,
	_spreadSheetIndex: 0,
	_maxZIndex: 5,
	
	tmp: {
		id: "",
		diffX: 0,
		diffY: 0,
		rect: null,
		//startLeft: 0,
		//startTop: 0,
		moveMode: false,
		boundaryMode: false,
	},

	getNextCircleIndex: function(){
		++this._circleIndex;
		return this._circleIndex;
	},
	
	getNextArrowIndex: function(){
		++this._arrowIndex;
		return this._arrowIndex;
	},
	
	 getNextTextareaIndex: function(){
	 	++this._textareaIndex;
	 	return this._textareaIndex;
	 },
	 
	  getNextImageViewIndex: function(){
	  	++this._imageViewIndex;
	  	return this._imageViewIndex;
	  },
	  
	  getNextSpreadSheetIndex: function(){
	  	++this._spreadSheetIndex;
	  	return this._spreadSheetIndex;
	  },
	  
	  getNextZIndex: function(){
	  	++this._maxZIndex;
	  	return this._maxZIndex;
	  }	
};

(function(){

var mySize = {
	canvasMargin: 2,
	leftCanvasWidth: 33,
	topRangeHeight: 25,
	circleRadius: 10,
	arrowWidth: 2,
	modeImageWidth: 30,
	modeImageLeftMargin: 3,
	modeImageHeightDiff: 35,
	modeImageStartHeight: 15,
	textareaHeaderWidth: 130,
	textareaHeaderHeight: 10,
	defaultImageMaxWidth: 300
};

var myColor = {
	top1: 				"#7D7B7B",
	top2: 				"#504E4E",
	top3: 				"#333333",
	left1: 				"#C0EDF9",
	left2: 				"#2F9C9C",
	left3: 				"#1D5173",
	right1: 			"#DCDCDC",
	right2: 			"#FAFAD2",
	right3: 			"#B0C4DE",
	circleFill: 		"#ffffff",
	circleBoudary: 		"#000000",
	circleSelectedFill: "#F8C4C4",
	arrowStroke: 		"#000000",
	textareaBackground: "#F8FABF",
	textareaHeader1: 	"#AAEBC3",
	textareaHeader2: 	"#5EE793",
	textareaHeader3: 	"#30D56F",
};

var myId = {
	topRangeDiv:		"topRangeDiv",
	topRangeCanvas:		"topRangeCanvas",
	leftRangeDiv: 		"leftRangeDiv",
	leftRangeCanvas: 	"leftRangeCanvas",
	rightRangeDiv: 		"rightRangeDiv",
	rightRangeCanvas: 	"rightRangeCanvas",
	
	arrowListDiv: 		"arrowListDiv",
	circleListDiv: 		"circleListDiv",
	textareaListDiv:	"textareaListDiv",
	imageViewListDiv:	"imageViewListDiv",
	spreadSheetListDiv:	"spreadSheetListDiv",	
	
	nodeModeImage: 		"nodeModeImage",
	arrowModeImage: 	"arrowModeImage",
	textAreaModeImage:	"textAreaModeImage",
	imageModeImage:		"imageModeImage",
	spreadSheetModeImage: "spareadSheetModeImage",
	
	sub : {
		circleCanvas: 			"circleCanvas_",
		circleCanvasDiv: 		"circleCanvasDiv_",
		arrowCanvas: 			"arrowCanvas_",
		arrowCanvasDiv: 		"arrowCanvasDiv_",
		textareaHeaderCanvas:	"textareaHeaderCanvas_",
		textareaCanvasDiv:		"textareaCanvasDiv_",
		textarea:				"textarea_",
		imageViewDiv:			"imageViewDiv_",
		imageviewImage:			"imageViewImage_",
		imageFileInput:			"imageFileInput_"
	}
};

var myImageFile = {
	circleSelect: 			"images/nodeSelect.jpg",
	circleNotSelect: 		"images/nodeNotSelect.jpg",
	circleDisable: 			"images/nodeDisable.jpg",
	arrowSelect: 			"images/arrowSelect.jpg",
	arrowNotSelect: 		"images/arrowNotSelect.jpg",
	arrowDisable: 			"images/arrowDisable.jpg",
	textareaSelect: 		"images/textAreaSelect.jpg",
	textareaNotSelect: 		"images/textAreaNotSelect.jpg",
	textareaDisable: 		"images/textAreaDisable.jpg",
	imageSelect:			"images/imageSelect.jpg",
	imageNotSelect:			"images/imageNotSelect.jpg",
	imageDisable:			"images/imageDisable.jpg",
	spreadSheetSelect:		"images/spreadSheetSelect.jpg",
	spreadSheetNotSelect: 	"images/spreadSheetNotSelect.jpg",
	spreadSheetDisable:		"images/spareadSheetDisable.jpg"
};

var Point = function(x, y){
	this.x = x;
	this.y = y;
};

var Component = {
	Node: function(divJElem){
		this.divJElem = divJElem;
		this.radius = 0;
		this.inArrows = [];
		this.outArrows = [];
		
		this.isInArrow = function(arrow){
			for(var i = 0; i < this.inArrows.length; i++){
				if(arrow.startNode == this.inArrows[i].startNode
					&& arrow.endNode == this.inArrows[i].endNode){
					return true;
				}
			}
			return false;
		};
		
		this.isOutArrow = function(arrow){
			for(var i = 0; i < this.outArrows.length; i++){
				if(arrow.startNode == this.outArrows[i].startNode
					&& arrow.endNode == this.outArrows[i].endNode){
					return true;
				}
			}
			return false;
		};
		
		this.isAdjucent = function(node){
			for(var i = 0; i < this.outArrows.length; i++){
				if(node == this.outArrows[i].endNode){
					return true;
				}
			}
			for(var i = 0; i < this.inArrows.length; i++){
				if(node == this.inArrows[i].startNode){
					return true;
				}
			}
			return false;
		};
		
		this.addInArrow = function(arrow){
			if(this.isInArrow(arrow) || this.isOutArrow(arrow)){
				return;
			}
			this.inArrows.push(arrow);
		};
		
		this.addOutArrow = function(arrow){
			if(this.isInArrow(arrow) || this.isOutArrow(arrow)){
				return;
			}
			this.outArrows.push(arrow);
		};
		
		this.getCenterPos = function(){
			var rect = this.divJElem[0].getBoundingClientRect();
			var centerX = Math.round((rect.right + rect.left) / 2);
			var centerY = Math.round((rect.top + rect.bottom) / 2);
			return new Point(centerX, centerY);
		};
	},
	
	Arrow: function(divJElem, startNode, endNode){
		this.divJElem = divJElem;
		this.startNode = startNode;
		this.endNode = endNode;
		
		this.clearView = function(){
			var canvas = this.divJElem.find("canvas")[0];
			var width = canvas.width;
			var height = canvas.height;
			var ctx = canvas.getContext("2d");
			ctx.clearRect(0, 0, width, height);
			ctx.save();
			ctx.restore();
		};
		
		this.reDraw = function(){
			// 現在の矢印の表示をクリア
			this.clearView();
			
			// 再描画
			{
				var sp = this.startNode.getCenterPos();
				var ep = this.endNode.getCenterPos();
				
				var minX = Math.min(sp.x, ep.x);
				var maxX = Math.max(sp.x, ep.x);
				var minY = Math.min(sp.y, ep.y);
				var maxY = Math.max(sp.y, ep.y);
				var width = maxX - minX;
				var height = maxY - minY;
				
				this.divJElem.css("left", (minX - 1)+ "px")
					 	  	 .css("top", (minY - 1) + "px");
						 
				var canvas = this.divJElem.find("canvas");
				canvas.attr({width: width + 2, height: height + 2});
				
				sp.x -= minX;
				sp.y -= minY;
				ep.x -= minX;
				ep.y -= minY;
				
				ComponentCreator.createArrow(canvas[0], sp, ep);
			}
		};
	},
	
	TextArea: function(divJElem){
		this.divJElem = divJElem;
	},
	
	ImageView: function(divJElem){
		this.divJElem = divJElem;
	},
};

var Initializer = function(){
	
	this.createInitialCanvas = function(){
		var body = $("body");
		
		var brawserWidth = $(window).width();
		var brawserHeight = $(window).height();
		
		// topRange
		{			
			var topElem = $("<div>", {"id": myId.topRangeDiv});
			var topCanvas = $("<canvas>", {"id": myId.topRangeCanvas})
								.css("margin", "0px " + mySize.canvasMargin + "px 0px " + mySize.canvasMargin + "px");
			topElem.append(topCanvas);
			body.append(topElem);
		}
		
		// leftRange
		{
			var leftElem = $("<div>", {"id": myId.leftRangeDiv});
			var canvasLeft = $("<canvas>", {"id": myId.leftRangeCanvas})
								.css("float", "left")
								.css("margin", mySize.canvasMargin + "px " + mySize.canvasMargin + "px " + mySize.canvasMargin + "px " + mySize.canvasMargin + "px");
			leftElem.append(canvasLeft);
			body.append(leftElem);
		}
		
		// rightRange
		{
			var rightElem = $("<div>", {"id": myId.rightRangeDiv})
								.bind("dragover", function(e){
									e.preventDefault();
								})
								.bind("drop", function(e){
									var id = gRightCanvas.tmp.id;
									var target = $("#" + id);
									target.css("left", (e.originalEvent.clientX - gRightCanvas.tmp.diffX) + "px")
										  .css("top", (e.originalEvent.clientY - gRightCanvas.tmp.diffY) + "px")
										  .css("z-index", gRightCanvas.getNextZIndex());
									if(id.indexOf(myId.sub.circleCanvasDiv) != -1){
										NodeUtil.updateArrows(target);	
									}									
									e.preventDefault();
								})
								.bind("mousemove", function(e){
									if(gRightCanvas.tmp.moveMode){
										var id = gRightCanvas.tmp.id;
										var target = $("#" + id);
										target.css("left", (e.originalEvent.clientX - gRightCanvas.tmp.diffX) + "px")
										  	  .css("top", (e.originalEvent.clientY - gRightCanvas.tmp.diffY) + "px");
									} else if(gRightCanvas.tmp.boundaryMode){
										var width = ComponentCreator.getNextWidth(e.originalEvent.clientX);
										$("#" + gRightCanvas.tmp.id).attr({width: width});
									}
								})
								.bind("mouseup", function(e){
									gRightCanvas.tmp.moveMode = false;
									gRightCanvas.tmp.boundaryMode = false;
								});
			var canvasRight = $("<canvas>", {"id": myId.rightRangeCanvas})
								.css("float", "right")
								.css("margin", mySize.canvasMargin + "px " + mySize.canvasMargin + "px " + mySize.canvasMargin + "px " + mySize.canvasMargin + "px");				
			rightElem.append(canvasRight);	
			body.append(rightElem);
			
			// ArrowListDiv
			{
				var arrowListDiv = $("<div>", {"id": myId.arrowListDiv});
				rightElem.append(arrowListDiv);
			}
			
			// CircleListDiv
			{
				var circleListDiv = $("<div>", {"id": myId.circleListDiv});
				rightElem.append(circleListDiv);
			}
			
			// textareaListDiv
			{
				var textareaListDiv = $("<div>", {"id": myId.textareaListDiv});
				rightElem.append(textareaListDiv);
			}
			
			// imageViewListDiv
			{
				var imageViewListDiv = $("<div>", {"id": myId.imageViewListDiv});
				rightElem.append(imageViewListDiv);
			}
			
			// spreadSheetListDiv
			{
				var spreadSheetListDiv = $("<div>", {"id": myId.spreadSheetListDiv});
				rightElem.append(spreadSheetListDiv);
			}
		}
		
		// 描画更新
		ScreenController.reDraw();
	};
	
	this.createModeSelectBar = function(){
		
		var createImageJElem = function(id, top, left, mode, src){
			return $("<image>", {"id": id})
							.attr("src", src)
							.attr({width: mySize.modeImageWidth})
							.css("position", "absolute")
							.css("top", top + "px")
							.css("left", left + "px")							
							.click(function(){ ScreenController.changeMode(mode); });			
		};
		
		var imageStartHeight = mySize.modeImageStartHeight;
		var heightDiff = mySize.modeImageHeightDiff;
		var leftMargin = mySize.modeImageLeftMargin;
		
		var leftArea = $("#" + myId.leftRangeDiv);
		
		// 画像領域の生成
		{
			// circle
			var height = mySize.topRangeHeight + imageStartHeight;
			var circleImage = createImageJElem(myId.nodeModeImage, height, leftMargin, ComponentMode.Node, myImageFile.circleSelect);
			leftArea.append(circleImage);
			
			// arrow
			height += heightDiff;
			var arrowImage = createImageJElem(myId.arrowModeImage, height, leftMargin, ComponentMode.Arrow, myImageFile.arrowNotSelect);
			leftArea.append(arrowImage);
			
			// textarea
			height += heightDiff;
			var textAreaImage = createImageJElem(myId.textAreaModeImage, height, leftMargin, ComponentMode.TextArea, myImageFile.textareaNotSelect);
			leftArea.append(textAreaImage);
			
			// imageView
			height += heightDiff;
			var imageViewImage = createImageJElem(myId.imageModeImage, height, leftMargin, ComponentMode.ImageView, myImageFile.imageNotSelect);
			leftArea.append(imageViewImage);
			
			// spreadSheet
			height += heightDiff;
			var spreadSheetImage = createImageJElem(myId.spreadSheetModeImage, height, leftMargin, ComponentMode.SpreadSheet, myImageFile.spreadSheetNotSelect);
			leftArea.append(spreadSheetImage);
		}
	};
};

var ComponentCreator = {

	addGradation: function(canvas, width, height, color1, color2, color3, isVertical) {
		var ctx = canvas.getContext("2d");
		ctx.beginPath();
		var grad;
		if(isVertical){
			grad = ctx.createLinearGradient(0, 0, 0, height);
		} else {
			grad = ctx.createLinearGradient(0, 0, width, 0);
		}
		grad.addColorStop(0.0, color1);
		grad.addColorStop(0.5, color2);
		grad.addColorStop(1.0, color3);
		ctx.fillStyle = grad;
		ctx.rect(0, 0, width, height);
		ctx.fill();	
	},

	createCircleDivElem: function(absX, absY, radius, fillColor){	
		var id = gRightCanvas.getNextCircleIndex();
		var elem = $("<div>", {"id": myId.sub.circleCanvasDiv + id})
						.attr("draggable", "true")
						.css("position", "absolute")
						.css("left", (absX - radius) + "px")
						.css("top", (absY - radius) + "px")
						.css("z-index", "2")
						.bind("dragstart", function(e){ ComponentCreator.onDragStartCommon(e); });
						
		var canvas = $("<canvas>", {"id": myId.sub.circleCanvas + id})
						.attr({width: 2 * (radius + 1), height: 2 * (radius + 1)})
						//.css("background-color", "#00ff00")
						.css("margin", "0px 0px 0px 0px");
		
		if(fillColor == undefined){
			fillColor = myColor.circleFill;
		}
		
		this.createCircle(canvas[0], radius + 1, radius + 1, radius, fillColor);
		elem.append(canvas);
		
		return elem;
	},

	createCircle: function(canvas, x, y, radius, fillColor, strokeColor){
		var ctx = canvas.getContext("2d");
		
		if(fillColor == undefined){
			fillColor = myColor.circleFill;
		}
		
		if(strokeColor == undefined){
			strokeColor = myColor.circleBoudary;
		}
		
		ctx.beginPath();
		ctx.fillStyle = fillColor;
		ctx.arc(x, y, radius, 0, Math.PI * 2, false);
		ctx.fill();
		ctx.beginPath();
		ctx.strokeSTyle = strokeColor;
		ctx.lineWidth = 2;
		ctx.arc(x, y, radius, 0, Math.PI * 2, false);
		ctx.stroke();
	},

	createArrowDivElem: function(startNode, endNode){
		
		var sp = startNode.getCenterPos();
		var ep = endNode.getCenterPos();
		
		var minX = Math.min(sp.x, ep.x);
		var maxX = Math.max(sp.x, ep.x);
		var minY = Math.min(sp.y, ep.y);
		var maxY = Math.max(sp.y, ep.y);
		var width = maxX - minX;
		var height = maxY - minY;
		
		var id = gRightCanvas.getNextArrowIndex();
		var elem = $("<div>", {"id": myId.sub.arrowCanvasDiv + id})
						.css("position", "absolute")
						.css("left", (minX - 1) + "px")
						.css("top", (minY - 1) + "px")
						.css("z-index", "1");
		var canvas = $("<canvas>", {"id": myId.sub.arrowCanvas + id})
						.attr({width: width + 2, height: height + 2})
						//.css("background-color", "#ff0000")
						.css("margin", "0px 0px 0px 0px");
		
		sp.x -= minX;
		sp.y -= minY;
		ep.x -= minX;
		ep.y -= minY;
	
		this.createArrow(canvas[0], sp, ep);
		elem.append(canvas);
		
		return elem;
	},

	createArrow: function(canvas, startPos, endPos){
		var ctx = canvas.getContext("2d");
		ctx.beginPath();
		ctx.lineWidth = mySize.arrowWidth;
		ctx.strokeStyle = myColor.arrowStroke;
		ctx.moveTo(startPos.x, startPos.y);
		ctx.lineTo(endPos.x, endPos.y);
		ctx.stroke();
	},

	createTextAreaDivElem: function(clientX, clientY){
		var id = gRightCanvas.getNextTextareaIndex();
		var elem = $("<div>", {"id": myId.sub.textareaCanvasDiv + id})
						.attr("draggable", "true")
						.css("position", "absolute")
						.css("left", clientX + "px")
						.css("top", clientY + "px")
						.css("z-index", "5")
						.bind("dragstart", function(e){ ComponentCreator.onDragStartCommon(e); });
						
		var textarea = $("<textarea>", {"id": myId.sub.textarea + id})
						.attr({cols: 20, rows: 5})
						//.attr({width: 200, height: 150})
						.css("position", "absolute")
						.css("left", "0px")
						.css("top", 14 + "px")
						.css("background-color", myColor.textareaBackground)
						.css("margin", "0px 0px 0px 0px");
						
		var canvas = $("<canvas>", {"id": myId.sub.textareaHeaderCanvas + id})
						.attr({width: mySize.textareaHeaderWidth, height: mySize.textareaHeaderHeight})
						.css("margin", "0px 0px 0px 0px");
		this.addGradation(canvas[0], mySize.textareaHeaderWidth, mySize.textareaHeaderHeight, myColor.textareaHeader1, myColor.textareaHeader2, myColor.textareaHeader3, false);
						
		elem.append(canvas);
		elem.append(textarea);
		
		return elem;
	},
	
	onDragStartCommon: function(e){
		gRightCanvas.tmp.id = e.target.id;
		var rect = e.target.getBoundingClientRect();
		gRightCanvas.tmp.diffX = e.originalEvent.clientX - rect.left;
		gRightCanvas.tmp.diffY = e.originalEvent.clientY - rect.top;
	},
	
	isRectBoundary: function(rect, clientX, clientY, boundaryWidth){		
		return (clientX < rect.left 	+ boundaryWidth
			 || clientX > rect.right 	- boundaryWidth
			 || clientY < rect.top 		+ boundaryWidth
			 || clientY > rect.bottom 	- boundaryWidth);
	},
	
	imageOnMouseDown: function(e){
		var elem = e.target.parentNode;
		var rect = elem.getBoundingClientRect();
		
		// boundaryをクリックした場合はサイズを変更
		if(ComponentCreator.isRectBoundary(rect, e.originalEvent.clientX, e.originalEvent.clientY, 5)){
			gRightCanvas.tmp.rect = rect;
			gRightCanvas.tmp.diffX = e.originalEvent.clientX - rect.left;
			gRightCanvas.tmp.diffY = e.originalEvent.clientY - rect.top;
			gRightCanvas.tmp.boundaryMode = true;
			gRightCanvas.tmp.moveMode = false;
			gRightCanvas.tmp.id = e.target.id;
		// 内部をクリックした場合は移動する
		} else if(gRightCanvas.tmp.boundaryMode == false){
			gRightCanvas.tmp.diffX = e.originalEvent.clientX - rect.left;
			gRightCanvas.tmp.diffY = e.originalEvent.clientY - rect.top;
			$(elem).css("z-index", gRightCanvas.getNextZIndex());
			gRightCanvas.tmp.moveMode = true;
			gRightCanvas.tmp.boundaryMode = false;
			gRightCanvas.tmp.id = elem.id;
		}
	},
	
	imageOnMouseUp: function(e){
		gRightCanvas.tmp.moveMode = false;
		gRightCanvas.tmp.boudnaryMode = false;
	},
	
	getNextWidth: function(clickX){
		var virtualLeft = clickX - gRightCanvas.tmp.diffX;
		var plusWidth = virtualLeft - gRightCanvas.tmp.rect.left;
		var width = gRightCanvas.tmp.rect.right - gRightCanvas.tmp.rect.left + plusWidth;
		return width;
	},	
	
	imageOnMouseMove: function(e){
		var isLButtonDown = (window.event.which == 1) ? true : false;
		if(gRightCanvas.tmp.moveMode && isLButtonDown){
			$("#" + gRightCanvas.tmp.id).css("left", (e.originalEvent.clientX - gRightCanvas.tmp.diffX) + "px")
			   	   						.css("top", (e.originalEvent.clientY - gRightCanvas.tmp.diffY) + "px");								       
		} else if(gRightCanvas.tmp.boundaryMode && isLButtonDown){
			var width = ComponentCreator.getNextWidth(e.originalEvent.clientX);
			$("#" + gRightCanvas.tmp.id).attr({width: width});
		} else {
			gRightCanvas.tmp.moveMode = false;
			gRightCanvas.tmp.boudnaryMode = false;
		}
	},
	
	creeateImageViewDivElem: function(clientX, clientY){		
		var id = gRightCanvas.getNextImageViewIndex();
		var elem = $("<div>", {"id": myId.sub.imageViewDiv + id})
						.css("position", "absolute")
						.css("left", clientX + "px")
						.css("top", clientY + "px")
						.css("z-index", "3")
						.bind("mousedown", function(e){ ComponentCreator.imageOnMouseDown(e); })
						.bind("mouseup",   function(e){ ComponentCreator.imageOnMouseUp(e);   })
						.bind("mousemove", function(e){ ComponentCreator.imageOnMouseMove(e); })
						.bind("dragstart", function(e){
							ComponentCreator.imageOnMouseDown(e);
							e.preventDefault();
						});
	
		var imageView = $("<image>", {"id": myId.sub.imageviewImage + id})
							.css("margin", "0px 0px 0px 0px");
							
		var input = $("<input>", {"id": "imageFileInput_" + id, "type": "file"})
						.attr("style", "display:none")
						.bind("change", function(e){
							var file = $(this).prop("files")[0];
							var fr = new FileReader();
							fr.onload = function(){
								imageView.attr("src", fr.result);
								
								// 画像が大木素ぶる場合は、初期状態は縮小表示
								var width = imageView[0].naturalWidth;
								if(width > mySize.defaultImageMaxWidth){
									imageView.attr({width: mySize.defaultImageMaxWidth});	
								}								
							};
							fr.readAsDataURL(file);
						});		
		
		elem.append(input);
		elem.append(imageView);
		
		return elem;
	},
};

var ComponentMode = {
	Node: 1,
	Arrow: 2,
	TextArea: 3,
	ImageView: 4,
	SpreadSheet: 5,
};

var RandomUtil = {
	getRondomValue: function(size){
		return Math.floor(Math.random() * size);
	},
	
	getRandomDarkerColor: function(){
		var r = this.getRondomValue(100) + 25;
		var g = this.getRondomValue(100) + 25;
		var b = this.getRondomValue(100) + 25;
		return "rgb(" + r + ", " + g + ", " + b + ")";
	},
	
	getRandomThickColor: function(){
		var r = this.getRondomValue(100) + 125;
		var g = this.getRondomValue(100) + 125;
		var b = this.getRondomValue(100) + 125;
		return "rgb(" + r + ", " + g + ", " + b + ")";
	},

	getRondomRadius: function(maxSize){
		return this.getRondomValue(maxSize) + 3;
	},
};

var NodeUtil = {
	getNode: function(nodeJDiv){
		for(var i = 0; i < gMgr.nodeList.length; ++i){
			if(gMgr.nodeList[i].divJElem[0].id == nodeJDiv[0].id){
				return gMgr.nodeList[i];
			}
		}
		return null;
	},
	
	getNodeByJDiv: function(divElem){
		for(var i = 0; i < gMgr.nodeList.length; ++i){
			if(gMgr.nodeList[i].divJElem[0].id == divElem.id){
				return gMgr.nodeList[i];
			}
		};
		return null;
	},
	
	updateArrows: function(moveNodeJDiv){
		var moveNode = this.getNode(moveNodeJDiv);
		for(var i = 0; i < moveNode.inArrows.length; ++i){
			moveNode.inArrows[i].reDraw();
		}
		
		for(var i = 0; i < moveNode.outArrows.length; ++i){
			moveNode.outArrows[i].reDraw();
		}
	},

	selectNode: function(node, isSelect){
		var canvas = node.divJElem.find("canvas");
		if(isSelect){
			ComponentCreator.createCircle(canvas[0], node.radius + 1, node.radius + 1, node.radius, myColor.circleSelectedFill);
		} else {
			ComponentCreator.createCircle(canvas[0], node.radius + 1, node.radius + 1, node.radius, myColor.circleFill);
		}
	},

	changeNodeSelection: function(fromNode, toNode){
		this.selectNode(fromNode, false);
		this.selectNode(toNode, true);
		gMgr.arrowStartNode = toNode;
	},
};

var MouseEventMgr = {
	setEvent: function(mode){
		switch(mode){
			case ComponentMode.Node:
				this.setNodeClickEvent();
				break;
			case ComponentMode.Arrow:
				this.setArrowClickEvent();
				break;
			case ComponentMode.TextArea:
				this.setTextAreaClickEvent();
				break;
			case ComponentMode.ImageView:
				this.setImageViewClickEvent();
				break;
			case ComponentMode.SpreadSheet:
				this.setSpreadSheetClickEvent();
				break;
			default:
				break;
		}
	},
	
	createAndAddCircle: function(clientX, clientY){
		var radius = mySize.circleRadius;				
		var elem = ComponentCreator.createCircleDivElem(clientX, clientY, radius);
		var node = new Component.Node(elem);
		node.radius = radius;
		
		gMgr.nodeList.push(node);		
		$("#" + myId.circleListDiv).append(elem);
	},
	
	createAndAddTextArea: function(clientX, clientY){
		var elem = ComponentCreator.createTextAreaDivElem(clientX, clientY);
		var textarea = new Component.TextArea(elem);
		gMgr.textareaList.push(textarea);
		$("#" + myId.textareaListDiv).append(elem);
	},
	
	createAndAddImage: function(clientX, clientY){
		var elem = ComponentCreator.creeateImageViewDivElem(clientX, clientY);
		var imageView = new Component.ImageView(elem);
		gMgr.imageViewList.push(imageView);
		$("#" + myId.imageViewListDiv).append(elem);
		
		// file select dialog
		elem.find("input")[0].click();
	},
	
	setNodeClickEvent: function(){
		// right canvas click
		$("#" + myId.rightRangeCanvas).unbind("click");
		$("#" + myId.rightRangeCanvas).click(function(e){
			MouseEventMgr.createAndAddCircle(e.clientX, e.clientY);
		});
		
		// arrow div click
		for(var i = 0; i < gMgr.arrowList.length; ++i){
			gMgr.arrowList[i].divJElem.unbind("click");
			gMgr.arrowList[i].divJElem.click(function(e){
				MouseEventMgr.createAndAddCircle(e.clientX, e.clientY);
			});
		}
		
		// node click
		for(var i = 0; i < gMgr.nodeList.length; ++i){
			gMgr.nodeList[i].divJElem.unbind("click");
		}
	},
	
	setArrowClickEvent: function(){
		// right canvas click
		$("#" + myId.rightRangeCanvas).unbind("click");
		
		// arrow div click
		for(var i = 0; i < gMgr.arrowList.length; ++i){
			gMgr.arrowList[i].divJElem.unbind("click");
		}
		
		// node click
		for(var i = 0; i < gMgr.nodeList.length; i++){
			gMgr.nodeList[i].divJElem.click(function(e){
				// startNodeがセットされていないときはstartNodeにする
				if(gMgr.arrowStartNode == null){
					gMgr.arrowStartNode = NodeUtil.getNodeByJDiv(e.target.offsetParent);
					NodeUtil.selectNode(gMgr.arrowStartNode, true);
				} else {
					// 自身と一致するときは選択を解除する
					if(gMgr.arrowStartNode == NodeUtil.getNodeByJDiv(e.target.offsetParent)){
						NodeUtil.selectNode(gMgr.arrowStartNode, false);
						gMgr.arrowStartNode = null;
						return;
					}
					// 既に隣接関係にあるノードは登録しない
					var endNode = NodeUtil.getNodeByJDiv(e.target.offsetParent);
					if(gMgr.arrowStartNode.isAdjucent(endNode)){
						NodeUtil.changeNodeSelection(gMgr.arrowStartNode, endNode);
						return;
					}
					// Arrowを作成
					var elem = ComponentCreator.createArrowDivElem(gMgr.arrowStartNode, endNode);
					var arrow = new Component.Arrow(elem, gMgr.arrowStartNode, endNode);
					gMgr.arrowList.push(arrow);
					gMgr.arrowStartNode.addOutArrow(arrow);
					endNode.addInArrow(arrow);
					
					$("#" + myId.arrowListDiv).append(elem);
					
					// 選択対象を変更
					NodeUtil.changeNodeSelection(gMgr.arrowStartNode, endNode);
				}				
			});
		}
	},
	
	setTextAreaClickEvent: function(){
		// right canvas click
		$("#" + myId.rightRangeCanvas).unbind("click");
		$("#" + myId.rightRangeCanvas).click(function(e){
			MouseEventMgr.createAndAddTextArea(e.clientX, e.clientY);
		});
		
		// arrow div click
		for(var i = 0; i < gMgr.arrowList.length; ++i){
			gMgr.arrowList[i].divJElem.unbind("click");
			gMgr.arrowList[i].divJElem.click(function(e){
				MouseEventMgr.createAndAddTextArea(e.clientX, e.clientY);
			});
		}
		
		// node click
		for(var i = 0; i < gMgr.nodeList.length; i++){
			gMgr.nodeList[i].divJElem.unbind("click");
		}
	},
	
	setImageViewClickEvent: function(){
		// right canvas click
		$("#" + myId.rightRangeCanvas).unbind("click");
		$("#" + myId.rightRangeCanvas).click(function(e){
			MouseEventMgr.createAndAddImage(e.clientX, e.clientY);
		});
		
		// arrow div click
		for(var i = 0; i < gMgr.arrowList.length; ++i){
			gMgr.arrowList[i].divJElem.unbind("click");
			gMgr.arrowList[i].divJElem.click(function(e){
				MouseEventMgr.createAndAddImage(e.clientX, e.clientY);
			});
		}
		
		// node click
		for(var i = 0; i < gMgr.nodeList.length; i++){
			gMgr.nodeList[i].divJElem.unbind("click");
		}
	},
	
	setSpreadSheetClickEvent: function(){
		// right canvas click
		$("#" + myId.rightRangeCanvas).unbind("click");
		
		// arrow div click
		for(var i = 0; i < gMgr.arrowList.length; ++i){
			gMgr.arrowList[i].divJElem.unbind("click");
		}
		
		// node click
		for(var i = 0; i < gMgr.nodeList.length; i++){
			gMgr.nodeList[i].divJElem.unbind("click");
		}
	}
};

var ScreenController = {
	
	reDraw: function(){
		// top range を再描画
		this.reDrawTopRange();
		
		// left range　を再描画
		this.reDrawLeftRange();
		
		// right range　を再描画
		this.reDrawRightRange();
	},
	
	getBrowserWidth: function(){
		return $(window).width();
		//return $(document).width();
	},
	
	getBrowserHeight: function(){
		return $(window).height();
		//return $(document).height();
	},	
		
	// top range　を再描画
	reDrawTopRange: function(){
		var brawserWidth = this.getBrowserWidth();
		var brawserHeight = this.getBrowserHeight();
		
		// top range を再描画
		var topWidth = brawserWidth - mySize.canvasMargin * 2;
		var topHeight = mySize.topRangeHeight;
		$("#" + myId.topRangeCanvas).attr({width: topWidth, height: topHeight});
		ComponentCreator.addGradation($("#" + myId.topRangeCanvas)[0], topWidth, topHeight, myColor.top1, myColor.top2, myColor.top3, false);
	},
	
	// left range　を再描画
	reDrawLeftRange: function(){
		var brawserWidth = this.getBrowserWidth();
		var brawserHeight = this.getBrowserHeight();

		// left range　を再描画
		var leftWidth = mySize.leftCanvasWidth;
		var leftHeight = brawserHeight - mySize.canvasMargin * 3 - mySize.topRangeHeight;
		$("#" + myId.leftRangeCanvas).attr({width: leftWidth, height: leftHeight});
		ComponentCreator.addGradation($("#" + myId.leftRangeCanvas)[0], leftWidth, leftHeight, myColor.left1, myColor.left2, myColor.left3, true);
	},
	
	// right range　を再描画
	reDrawRightRange: function(){
		var brawserWidth = this.getBrowserWidth();
		var brawserHeight = this.getBrowserHeight();
		
		var rightWidth = brawserWidth - mySize.leftCanvasWidth - mySize.canvasMargin * 4;
		var rightHeight = brawserHeight - mySize.canvasMargin * 3 - mySize.topRangeHeight;
		$("#" + myId.rightRangeCanvas).attr({width: rightWidth, height: rightHeight});
		ComponentCreator.addGradation($("#" + myId.rightRangeCanvas)[0], rightWidth, rightHeight, myColor.right1, myColor.right2, myColor.right3, true);	
	},
	
	changeMode: function(mode){
		// modeが変わらない場合は何もしない
		if(gMgr.currMode == mode){
			return;
		}
		
		// 昔の選択状態を解除する
		switch(gMgr.currMode){
			case ComponentMode.Node: 		$("#" + myId.nodeModeImage).attr("src", 		myImageFile.circleNotSelect); 		break;
			case ComponentMode.Arrow: 		$("#" + myId.arrowModeImage).attr("src", 		myImageFile.arrowNotSelect); 		break;
			case ComponentMode.TextArea: 	$("#" + myId.textAreaModeImage).attr("src", 	myImageFile.textareaNotSelect); 	break;
			case ComponentMode.ImageView:	$("#" + myId.imageModeImage).attr("src", 		myImageFile.imageNotSelect); 		break;
			case ComponentMode.SpreadSheet: $("#" + myId.spreadSheetModeImage).attr("src", 	myImageFile.spreadSheetNotSelect); 	break;
			default:
				break;
		}
		
		// 変更先を選択
		switch(mode){
			case ComponentMode.Node: 		$("#" + myId.nodeModeImage).attr("src", 		myImageFile.circleSelect); 		break;
			case ComponentMode.Arrow: 		$("#" + myId.arrowModeImage).attr("src", 		myImageFile.arrowSelect); 		break;
			case ComponentMode.TextArea: 	$("#" + myId.textAreaModeImage).attr("src", 	myImageFile.textareaSelect); 	break;
			case ComponentMode.ImageView: 	$("#" + myId.imageModeImage).attr("src", 		myImageFile.imageSelect); 		break;
			case ComponentMode.SpreadSheet: $("#" + myId.spreadSheetModeImage).attr("src", 	myImageFile.spreadSheetSelect); break;
			default:
				break;
		}
		
		// 現在のモードをセット
		gMgr.currMode = mode;
		
		// マウスのイベントリスナを変更
		MouseEventMgr.setEvent(mode);
	},
	
	ImageViewListener: {
		
	}
};

window.onload = function(){
	
	var initializer = new Initializer();
	
	// canvasの生成
	initializer.createInitialCanvas();
	
	// 選択componentを生成
	initializer.createModeSelectBar();
	
	// 現在のモードをセット
	gMgr.currMode = ComponentMode.Node;
	
	// マウスのイベントリスナを変更
	MouseEventMgr.setEvent(gMgr.currMode);
};

window.onresize = function(){
	ScreenController.reDraw();
};

})();




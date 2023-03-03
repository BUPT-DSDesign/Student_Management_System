<template>
    <div>
        <div class="map" style="float:left;">
            <div id="container" style="width:500px;height:70vh;" />
            <div class="input-card">
                <div class="input-item">
                    <input type="button" class="btn" value="开始动画" id="start" @click="startAnimation()" />
                    <input type="button" class="btn" value="暂停动画" id="pause" @click="pauseAnimation()" />
                    <input type="button" class="btn" value="继续动画" id="resume" @click="resumeAnimation()" />
                    <input type="button" class="btn" value="停止动画" id="stop" @click="stopAnimation()" />
                </div>
            </div>
        </div>
        <div class="search" style="float:right;">
            <el-form label-width="40px">
                <el-form-item label="起点">
                    <el-input v-model="keyWord1" style="width:250px;"></el-input>
                </el-form-item>
                <ul class="list-group">
                    <li v-for="(p, index) of filplacelist1" :key="index" @click="chooseaddress1($event)">
                        {{ p.address }}
                    </li>
                </ul>
                <el-form-item label="终点">
                        <el-input v-model="keyWord2" style="width:250px;"></el-input>
                    </el-form-item>
                    <ul class="list-group">
                        <li v-for="(p, index) of filplacelist2" :key="index" @click="chooseaddress2($event)">
                            {{ p.address }}
                        </li>
                    </ul>
                <el-form-item>
                    <el-button type="primary" @click="onSubmit">查询路线</el-button>
                </el-form-item>
            </el-form>
        </div>
    </div>
</template>
<script>
// 绘制线路需要的坐标
var lineArr = [[116.35530714718364, 39.96393072423919], [116.35542348293764, 39.964436412717816], [116.35600217544192, 39.9646045260412]];
// var lineArr = [[116.478935, 39.997761], [108.983569, 34.285675], [103.85094, 35.987496], [106.205794, 38.458831], [111.761777, 40.875595]]
export default {
    data() {
        return {
            firstArr: [116.35530714718364, 39.96393072423919],// 中心点/初始坐标
            keyWord1: '', //用户输入的关键字(起点)
            keyWord2: '', //用户输入的关键字(终点)
            filplacelist1: [], //模糊匹配后的列表（起点
            filplacelist2: [], //模糊匹配后的列表（终点
            //列表（全
            placelist: [
                { id: 0, address: "青年公寓" },
                { id: 1, address: "北邮锦江酒店" },
                { id: 2, address: "学十一公寓" },
                { id: 3, address: "学九公寓" },
                { id: 4, address: "留学生公寓" },
                { id: 5, address: "教九" },
                { id: 6, address: "学十公寓(西门)" },
                { id: 7, address: "学十公寓(正门)" },
                { id: 8, address: "学十公寓(东门)" },
                { id: 9, address: "快递站(邮政)" },
                { id: 10, address: "经管楼" },
                { id: 11, address: "学六公寓" },
                { id: 12, address: "科研楼" },
                { id: 13, address: "快递站(顺丰、京东)" },
                { id: 14, address: "学生食堂(入口)" },
                { id: 15, address: "学生食堂(出口)" },
                { id: 16, address: "学苑风味餐厅" },
                { id: 17, address: "物美超市" },
                { id: 18, address: "打印店" },
                { id: 19, address: "门店" },
                { id: 20, address: "浴室(正门)" },
                { id: 21, address: "浴室(西门)" },
                { id: 22, address: "综合服务楼" },
                { id: 23, address: "学生活动中心(南门)" },
                { id: 24, address: "学生活动中心(西门)" },
                { id: 25, address: "综合食堂(入口)" },
                { id: 26, address: "综合食堂(出口)" },
                { id: 27, address: "学五公寓" },
                { id: 28, address: "学三公寓" },
                { id: 29, address: "学八公寓" },
                { id: 30, address: "学四公寓" },
                { id: 31, address: "学十三公寓" },
                { id: 32, address: "学一公寓" },
                { id: 33, address: "学二公寓" },
                { id: 34, address: "鸿通楼" },
                { id: 35, address: "邮局" },
                { id: 36, address: "教四(西门)" },
                { id: 37, address: "教四(东门)" },
                { id: 38, address: "教四(南门)" },
                { id: 39, address: "西大门" },
                { id: 40, address: "校训石" },
                { id: 41, address: "主席像" },
                { id: 42, address: "主楼" },
                { id: 43, address: "教三(北门)" },
                { id: 44, address: "教三(西门)" },
                { id: 45, address: "教三(东门)" },
                { id: 46, address: "校车发车点" },
                { id: 47, address: "校医院" },
                { id: 48, address: "中门邮局" },
                { id: 49, address: "教二(西门)" },
                { id: 50, address: "教二(北门)" },
                { id: 51, address: "可信网络通信协同创新中心(创新楼)" },
                { id: 52, address: "体育场" },
                { id: 53, address: "南大门" },
                { id: 54, address: "东大门" },
                { id: 55, address: "学生发展中心" },
                { id: 56, address: "移动营业厅" },
                { id: 57, address: "档案馆" },
                { id: 58, address: "图书馆" },
                { id: 59, address: "篮球场" },
                { id: 60, address: "学29公寓" },
                { id: 61, address: "行政办公楼(小白楼)" },
                { id: 62, address: "教一楼(西门)" },
                { id: 63, address: "教一楼(南门)" },
                { id: 64, address: "教一楼(东门)" },
                { id: 65, address: "体育馆" },
                { id: 66, address: "游泳馆" },
                { id: 67, address: "科学会堂" },
            ]

        }
    },
    //监视用户输入关键词keyWord1的变化，
    watch: {
        keyWord1(newvalue) {
            //如果当前关键词为空
            if (newvalue === "") {
                this.filplacelist1 = [];
            }
            else {
                //利用filter函数筛选匹配上的元素
                this.filplacelist1 = this.placelist.filter((item) => {
                    return item.address.indexOf(newvalue) != -1;
                })
            }
        },
         keyWord2(newvalue) {
            //如果当前关键词为空
            if (newvalue === "") {
                this.filplacelist2 = [];
            }
            else {
                //利用filter函数筛选匹配上的元素
                this.filplacelist2 = this.placelist.filter((item) => {
                    return item.address.indexOf(newvalue) != -1;
                })
            }

        }
    },
    created() { },
    mounted() {
        setTimeout(() => {
            this.initMap() // 异步加载（否则报错initMap is not defined）
            // this.initroad()
        }, 1000)
    },
    methods: {
        onSubmit() {
            var startId = -1, endId = -1;
            for (let key in this.placelist) {
                if (this.placelist[key].address == this.keyWord1) {
                    startId = key;
                }
                for (let key in this.placelist) {
                    if (this.placelist[key].address == this.keyWord2) {
                        endId = key;
                    }
                }
                console.log(startId,this.placelist[startId].address, endId, this.placelist[endId].address);

            }
        },
        //点击ul自动填充input
        chooseaddress1(e) {
            this.keyWord1 = e.target.innerText;

        },
        chooseaddress2(e) {
            this.keyWord2 = e.target.innerText;

        },
        // 初始化地图
        initMap() {
            var that = this
            this.map = new AMap.Map('container', {
                resizeEnable: true, // 窗口大小调整
                center: this.firstArr,
            })
            // 添加maker
            this.marker = new AMap.Marker({
                map: this.map,
                position: this.firstArr,
                icon: 'https://webapi.amap.com/images/car.png',
                offset: new AMap.Pixel(-26, -13), // 调整图片偏移
                autoRotation: true, // 自动旋转
                angle: -90 // 图片旋转角度
            })
            that.initroad() //初始化路径
        },
        // 初始化轨迹
        initroad() {
            // 绘制还未经过的路线
            this.polyline = new AMap.Polyline({
                map: this.map,
                path: lineArr,
                showDir: true,
                strokeColor: '#28F', // 线颜色--浅蓝色
                // strokeOpacity: 1,     //线透明度
                strokeWeight: 6, // 线宽
                // strokeStyle: "solid"  //线样式
                lineJoin: 'round' // 折线拐点的绘制样式
            })
            // 绘制路过了的轨迹
            var passedPolyline = new AMap.Polyline({
                map: this.map,
                strokeColor: '#AF5',
                // strokeOpacity: 1,     //线透明度
                strokeWeight: 6 // 线宽
                // strokeStyle: "solid"  //线样式
            })
            this.marker.on('moving', function (e) {
                passedPolyline.setPath(e.passedPath);
            });
            this.map.setFitView() // 合适的视口
        },
        startAnimation() {
            this.marker.moveAlong(lineArr, 200);
        },

        pauseAnimation() {
            this.marker.pauseMove();
        },

        resumeAnimation() {
            this.marker.resumeMove();
        },
        stopAnimation() {
            this.marker.stopMove();
        },
    }
}
</script>
<style>
.btn {
    background-color: #409EFF;
    /* Green */
    border: none;
    color: white;
    padding: 10px 20px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 15px;
    margin: 4px 2px;
    cursor: pointer;
    transition-duration: 0.4s;
}

.btn:hover {
    box-shadow: 0 12px 16px 0 rgba(0, 0, 0, 0.24), 0 17px 50px 0 rgba(0, 0, 0, 0.19);
}

.map {
    margin-top: 20px;
}
.input-card{
    margin-top:50px;
}
.search {
    margin-right: 233px;
    margin-top: 20px;
}

.list-group {
    margin-top: -18px;
    margin-left: 41px;
    height:100px;
    overflow-y:scroll;
}
.list-group::-webkit-scrollbar{
    width:0;
}
.list-group li {
    list-style: none;
    padding: 1px;
    cursor: pointer;
}

.list-group li:hover {
    color: #1971c2;
}</style>
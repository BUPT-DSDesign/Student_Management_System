<template>
    <div>
        <div id="container" style="width:500px;height:80vh" />
        <div class="input-card">
            <h4>轨迹回放控制</h4>
            <div class="input-item">
                <input type="button" class="btn" value="开始动画" id="start" @click="startAnimation()" />
                <input type="button" class="btn" value="暂停动画" id="pause" @click="pauseAnimation()" />
            </div>
            <div class="input-item">
                <input type="button" class="btn" value="继续动画" id="resume" @click="resumeAnimation()" />
                <input type="button" class="btn" value="停止动画" id="stop" @click="stopAnimation()" />
            </div>
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
            console.log('submit!');
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
                strokeColor: '#77DDFF', // 线颜色--浅蓝色
                // strokeOpacity: 1,     //线透明度
                strokeWeight: 6, // 线宽
                // strokeStyle: "solid"  //线样式
                lineJoin: 'round' // 折线拐点的绘制样式
            })
            // 绘制路过了的轨迹
            var passedPolyline = new AMap.Polyline({
                map: this.map,
                strokeColor: '#00BBFF', // 线颜色-深蓝色
                // strokeOpacity: 1,     //线透明度
                strokeWeight: 6 // 线宽
                // strokeStyle: "solid"  //线样式
            })
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
.bgd {
    float: left;
    margin-left: 50px;
    margin-top: 10px;
    height: 500px;
    width: 350px;
    background-image: url(../assets/image/map.jpg);
    background-size: 100% 100%;
}

.search {
    margin-top: 50px;
    float: right;
}
</style>
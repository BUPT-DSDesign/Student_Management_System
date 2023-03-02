<template>
    <div>
        <div id="container" style="width:100%;height:90vh" />
        <!-- 
        <div class="bgd"></div>
        <div class="search">
            <el-form ref="form" :model="form" label-width="80px">
                <el-form-item label="活动名称">
                    <el-input v-model="form.name"></el-input>
                </el-form-item>
                <el-form-item label="活动区域">
                    <el-select v-model="form.region" placeholder="请选择活动区域">
                        <el-option label="区域一" value="shanghai"></el-option>
                        <el-option label="区域二" value="beijing"></el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="活动时间">
                    <el-col :span="11">
                        <el-date-picker type="date" placeholder="选择日期" v-model="form.date1"
                            style="width: 100%;"></el-date-picker>
                    </el-col>
                    <el-col class="line" :span="2">-</el-col>
                    <el-col :span="11">
                        <el-time-picker placeholder="选择时间" v-model="form.date2" style="width: 100%;"></el-time-picker>
                    </el-col>
                </el-form-item>
                <el-form-item label="即时配送">
                    <el-switch v-model="form.delivery"></el-switch>
                </el-form-item>
                <el-form-item>
                    <el-button type="primary" @click="onSubmit">立即创建</el-button>
                    <el-button>取消</el-button>
                </el-form-item>
            </el-form>
        </div> -->
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

            // form: {
            //     name: '',
            //     region: '',
            //     date1: '',
            //     date2: '',
            //     delivery: false,
            //     type: [],
            //     resource: '',
            //     desc: ''
            // }
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
            that.initroad()
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
                path: [[116.35530714718364, 39.96393072423919], [116.35542348293764, 39.964436412717816], [116.35600217544192, 39.9646045260412]],
                // strokeOpacity: 1,     //线透明度
                strokeWeight: 6 // 线宽
                // strokeStyle: "solid"  //线样式
            })
            this.map.setFitView() // 合适的视口
        }
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
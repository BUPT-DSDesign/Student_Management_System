<template>
    <div v-loading="loading" element-loading-text="正在为您检索您可能想去的地方" element-loading-spinner="el-icon-loading"
        element-loading-background="rgba(0, 0, 0, 0.8)">
        <div class="map" style="float:left; margin-right:20px">
            <div id="container" style="width:500px;height:70vh;box-shadow: 0px 5px 5px #c8c8c8; " />
            <div class="input-card">
                <div class="input-item">
                    <input type="button" class="btn" value="开始动画" id="start" @click="startAnimation()" />
                    <input type="button" class="btn" value="暂停动画" id="pause" @click="pauseAnimation()" />
                    <input type="button" class="btn" value="继续动画" id="resume" @click="resumeAnimation()" />
                    <input type="button" class="btn" value="停止动画" id="stop" @click="stopAnimation()" />
                </div>
            </div>
        </div>
        <div class="search" style="float:right margin-right:10px ">
            <h6>起始位置</h6>
            <el-autocomplete v-model="inputValue" :fetch-suggestions="querySearch" placeholder="请输入您当前的位置"
                @select="getInput" :trigger-on-focus="false" class="el-auto">
            </el-autocomplete>
            <div style="margin:10px 0">
                <h6> 请选择导航类型</h6>
                <el-radio v-model="radio" label="1">课程导航</el-radio>
                <el-radio v-model="radio" label="2">课外活动导航</el-radio>
                <el-radio v-model="radio" label="3">临时事务导航</el-radio>
            </div>
            <div v-show="radio == 1">
                <h6>您今天的课程有</h6>
                <el-table :data="curcourseList" style="width: 500px;height:300px;overflow-y: scroll">
                    <el-table-column label="节次" width="180">
                        <template slot-scope="scope">
                            <span style="margin-left: 10px">第{{ scope.row.time }}节</span>
                        </template>
                    </el-table-column>
                    <el-table-column label="课程名称" width="180">
                        <template slot-scope="scope">
                            <span style="margin-left: 10px">{{ scope.row.name }}</span>
                        </template>
                    </el-table-column>
                    <el-table-column label="操作">
                        <template slot-scope="scope">
                            <el-button size="mini" @click="submitclassNav(scope.$index, scope.row)">去上这节课</el-button>
                        </template>
                    </el-table-column>
                </el-table>
            </div>

            <div v-show="radio == 2">
                <h6>您今天的活动有</h6>
                <el-table :data="outEventList" style="width: 500px ;height:300px;overflow-y: scroll">
                    <el-table-column label="时间" width="180">
                        <template slot-scope="scope">
                            <span style="margin-left: 10px">{{ scope.row.time }}</span>
                        </template>
                    </el-table-column>
                    <el-table-column label="活动名称" width="180">
                        <template slot-scope="scope">
                            <span style="margin-left: 10px">{{ scope.row.name }}</span>
                        </template>
                    </el-table-column>
                    <el-table-column label="操作">
                        <template slot-scope="scope">
                            <el-button size="mini" @click="submitOutEventNav(scope.$index, scope.row)">去做这项活动</el-button>
                        </template>
                    </el-table-column>
                </el-table>
            </div>
            <div v-show="radio == 3">
                <h6>您今天的临时事务有</h6>
                <el-table :data="tempEventList" style="width: 500px ;height:300px;overflow-y: scroll"
                    @selection-change="handleSelectionChange">
                    <el-table-column type="selection" width="20">
                    </el-table-column>
                    <el-table-column label="时间" width="100">
                        <template slot-scope="scope">
                            <span style="margin-left: 10px">{{ scope.row.time }}</span>
                        </template>
                    </el-table-column>
                    <el-table-column label="临时事务名称" width="180">
                        <template slot-scope="scope">
                            <span style="margin-left: 10px">{{ scope.row.name }}</span>
                        </template>
                    </el-table-column>
                    <el-table-column label="地点" width="180">
                        <template slot-scope="scope">
                            <span style="margin-left: 10px">{{ scope.row.place }}</span>
                        </template>
                    </el-table-column>
                </el-table>
                <el-button @click="submitTempEventNav">去做这些活动</el-button>

            </div>
        </div>
        <el-button @click="testTsp">测试tsp问题</el-button>
    </div>
</template>
<script>
import { CourseStore } from '@/store/course';
import { TimeStore } from '@/store/time';
import { EventStore } from '@/store/event';
import { NavigateStore } from "@/store/navigate"
import { Col } from 'element-ui';
import { LogStore } from '@/store/log';

// 绘制线路需要的坐标
// var lineArr = [[116.35530714718364, 39.96393072423919], [116.35542348293764, 39.964436412717816], [116.35600217544192, 39.9646045260412]];
export default {
    data() {
        return {
            loading: false,
            firstArr: [116.35530714718364, 39.96393072423919],// 中心点/初始坐标
            // lineArr: [[116.35530714718364, 39.96393072423919], [116.35542348293764, 39.964436412717816], [116.35600217544192, 39.9646045260412]], // 路径上的点
            lineArr: [], // 路径上的点
            markers: [],
            radio: '1',//单选框的选择
            //列表（全
            placelist: [
                { id: 0, address: "青年公寓", value: "青年公寓" },
                { id: 1, address: "北邮锦江酒店", value: "北邮锦江酒店" },
                { id: 2, address: "学十一公寓", value: "学十一公寓" },
                { id: 3, address: "学九公寓", value: "学九公寓" },
                { id: 4, address: "留学生公寓", value: "留学生公寓" },
                { id: 5, address: "教九", value: "教九" },
                { id: 6, address: "学十公寓(西门)", value: "学十公寓(西门)" },
                { id: 7, address: "学十公寓(正门)", value: "学十公寓(正门)" },
                { id: 8, address: "学十公寓(东门)", value: "学十公寓(东门)" },
                { id: 9, address: "快递站(邮政)", value: "快递站(邮政)" },
                { id: 10, address: "经管楼", value: "经管楼" },
                { id: 11, address: "学六公寓", value: "学六公寓" },
                { id: 12, address: "科研楼", value: "科研楼" },
                { id: 13, address: "快递站(顺丰、京东)", value: "快递站(顺丰、京东)" },
                { id: 14, address: "学生食堂(入口)", value: "学生食堂(入口)" },
                { id: 15, address: "学生食堂(出口)", value: "学生食堂(出口)" },
                { id: 16, address: "学苑风味餐厅", value: "学苑风味餐厅" },
                { id: 17, address: "物美超市", value: "物美超市" },
                { id: 18, address: "打印店", value: "打印店" },
                { id: 19, address: "门店", value: "门店" },
                { id: 20, address: "浴室(正门)", value: "浴室(正门)" },
                { id: 21, address: "浴室(西门)", value: "浴室(西门)" },
                { id: 22, address: "综合服务楼", value: "综合服务楼" },
                { id: 23, address: "学生活动中心(南门)", value: "学生活动中心(南门)" },
                { id: 24, address: "学生活动中心(西门)", value: "学生活动中心(西门)" },
                { id: 25, address: "综合食堂(入口)", value: "综合食堂(入口)" },
                { id: 26, address: "综合食堂(出口)", value: "综合食堂(出口)" },
                { id: 27, address: "学五公寓", value: "学五公寓" },
                { id: 28, address: "学三公寓", value: "学三公寓" },
                { id: 29, address: "学八公寓", value: "学八公寓" },
                { id: 30, address: "学四公寓", value: "学四公寓" },
                { id: 31, address: "学十三公寓", value: "学十三公寓" },
                { id: 32, address: "学一公寓", value: "学一公寓" },
                { id: 33, address: "学二公寓", value: "学二公寓" },
                { id: 34, address: "鸿通楼", value: "鸿通楼" },
                { id: 35, address: "邮局", value: "邮局" },
                { id: 36, address: "教四(西门)", value: "教四(西门)" },
                { id: 37, address: "教四", value: "教四(东门)" },
                { id: 38, address: "教四(南门)", value: "教四(南门)" },
                { id: 39, address: "西大门", value: "西大门" },
                { id: 40, address: "校训石", value: "校训石" },
                { id: 41, address: "主席像", value: "主席像" },
                { id: 42, address: "主楼", value: "主楼" },
                { id: 43, address: "教三(北门)", value: "教三(北门)" },
                { id: 44, address: "教三(西门)", value: "教三(西门)" },
                { id: 45, address: "教三", value: "教三" },
                { id: 46, address: "校车发车点", value: "校车发车点" },
                { id: 47, address: "校医院", value: "校医院" },
                { id: 48, address: "中门邮局", value: "中门邮局" },
                { id: 49, address: "教二", value: "教二(西门)" },
                { id: 50, address: "教二(北门)", value: "教二(北门)" },
                { id: 51, address: "可信网络通信协同创新中心(创新楼)", value: "可信网络通信协同创新中心(创新楼)" },
                { id: 52, address: "体育场", value: "体育场" },
                { id: 53, address: "南大门", value: "南大门" },
                { id: 54, address: "东大门", value: "东大门" },
                { id: 55, address: "学生发展中心", value: "学生发展中心" },
                { id: 56, address: "移动营业厅", value: "移动营业厅" },
                { id: 57, address: "档案馆", value: "档案馆" },
                { id: 58, address: "图书馆", value: "图书馆" },
                { id: 59, address: "篮球场", value: "篮球场" },
                { id: 60, address: "学29公寓", value: "学29公寓" },
                { id: 61, address: "行政办公楼(小白楼)", value: "行政办公楼(小白楼)" },
                { id: 62, address: "教一", value: "教一楼(西门)" },
                { id: 63, address: "教一楼(南门)", value: "教一楼(南门)" },
                { id: 64, address: "教一楼(东门)", value: "教一楼(东门)" },
                { id: 65, address: "体育馆", value: "体育馆" },
                { id: 66, address: "游泳馆", value: "游泳馆" },
                { id: 67, address: "科学会堂", value: "科学会堂", },
            ],
            curcourseList: [],
            outEventList: [],
            tempEventList: [],
            courseList: [],
            eventList: [],
            inputValue: "", //用户输入的关键字
            multipleSelection: [],//用户多选选中的临时事务
        }
    },
    created() {
        const getTable1 = async () => {
            const fg = await await CourseStore.GetCourseTable()
            if (fg) {
                this.courseList = CourseStore.courseList;
                //根据当前周，查找在本周的课程
                this.courseList = this.courseList.filter((item) => {
                    return item.week_schedule.indexOf(TimeStore.week) != -1;
                });

                //查找本天的课程，然后将他们按照顺序排列。
                for (let i = 0; i < this.courseList.length; i++) {
                    for (let j = 0; j < this.courseList[i].section_list.length; j++) {
                        if (this.courseList[i].section_list[j] / 9 < TimeStore.day) {
                            console.log(this.courseList[i].section_list[j] / 9)
                            this.curcourseList.push({
                                name: this.courseList[i].course_name,
                                place: this.courseList[i].classroom,
                                time: this.courseList[i].section_list[j] % 9,
                            })
                        }
                        if (this.courseList[i].section_list[j] / 9 == TimeStore.day) {
                            this.curcourseList.push({
                                name: this.courseList[i].course_name,
                                place: this.courseList[i].classroom,
                                time: 9,
                            })
                        }
                    }
                }
                //对课程按照节次进行排序
                this.curcourseList.sort(function (a, b) {
                    return a.time - b.time;
                });
            } else {
                console.log('error')
            }
        };
        getTable1();

        const getTable2 = async () => {
            const fg = await await EventStore.GetEventTable()
            if (fg) {
                //筛选课外活动列表
                this.eventList = EventStore.eventList;
                for (let i = 0; i < this.eventList.length; i++) {
                    if (this.eventList[i].type == 0 && this.eventList[i].start_week == TimeStore.week && this.eventList[i].start_day == TimeStore.day) {
                        this.outEventList.push({
                            name: this.eventList[i].activity_name,
                            time: this.eventList[i].start_time,
                            place: this.eventList[i].location,
                        })
                    }
                }
                //筛选临时活动列表
                for (let i = 0; i < this.eventList.length; i++) {
                    if (this.eventList[i].type == 1 && this.eventList[i].start_week == TimeStore.week && this.eventList[i].start_day == TimeStore.day) {
                        this.tempEventList.push({
                            name: this.eventList[i].activity_name,
                            time: this.eventList[i].start_time,
                            place: this.eventList[i].location,
                        })
                    }
                }
            } else {
                console.log('error')
            }
        }
        getTable2()
    },
    mounted() {
        setTimeout(() => {
            this.initMap() // 异步加载（否则报错initMap is not defined）
        }, 1000);

    },
    methods: {
        testTsp() {
            let startId = 30
            let passIds = '{"0": 1, "1": 3, "2": 5, "3": 7, "4": 9, "5": 11, "6": 13, "7": 15, "8": 17, "9": 19, "10": 21, "11": 23, "12": 25, "13": 27, "14":29, "15": 31, "16":33, "17":35, "18":37, "19": 39}'
            const getPath = async () => {
                const flag = await NavigateStore.GetTSPPath(startId, passIds)
                if (flag) {
                    // 从pinia传来的数据
                    console.log(NavigateStore.tspPath)
                    this.lineArr = NavigateStore.tspPath.node_list
                    this.firstArr = this.lineArr[0];
                    let passlist = NavigateStore.tspPath.pass_list;
                    this.markers = [];
                    for (let i = 0; i < passlist.length; i++) {
                        this.markers.push({
                            icon: 'https://i.328888.xyz/2023/04/09/icaVW8.png',
                            // 设置了 icon 以后，设置 icon 的偏移量，以 icon 的 [center bottom] 为原点
                            offset: new AMap.Pixel(-13, -30),
                            position: passlist[i],
                        })
                    }
                    this.initMap();
                    this.nmarker();
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '寻路成功',
                        type: 'success'
                    });

                } else {
                    console.log('error')
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '寻路失败',
                        type: 'error'
                    });
                }
            }
            getPath()
        },
        // 初始化地图
        initMap() {
            var that = this
            this.map = new AMap.Map('container', {
                resizeEnable: true, // 窗口大小调整
                center: this.firstArr,
            })
            // 创建 AMap.Icon 实例：
            this.icon = new AMap.Icon({
                size: new AMap.Size(120, 70),    // 图标尺寸
                image: 'https://media0.giphy.com/media/6GdnEOYPA8JDsxAKg9/source.gif',  // Icon的图像
                imageOffset: new AMap.Pixel(0, 0),  // 图像相对展示区域的偏移量，适于雪碧图等
                imageSize: new AMap.Size(120, 70)   // 根据所设置的大小拉伸或压缩图片
            });
            // 添加maker
            this.marker = new AMap.Marker({
                map: this.map,
                position: this.firstArr,
                icon: this.icon, // 添加 Icon 实例
                offset: new AMap.Pixel(-60, -60), // 调整图片偏移
            })
            that.initroad() //初始化路径

        },
        // 初始化轨迹
        initroad() {
            // 绘制还未经过的路线
            this.polyline = new AMap.Polyline({
                map: this.map,
                path: this.lineArr,
                showDir: true,
                strokeColor: '#28F', // 线颜色--浅蓝色
                strokeWeight: 6, // 线宽
                lineJoin: 'round' // 折线拐点的绘制样式
            })
            // 绘制路过了的轨迹
            var passedPolyline = new AMap.Polyline({
                map: this.map,
                strokeColor: '#AF5',
                strokeWeight: 6 // 线宽
            })
            this.marker.on('moving', function (e) {
                passedPolyline.setPath(e.passedPath);
            });
            this.map.setFitView() // 合适的视口
        },
        nmarker() {
            var that = this;
            // 添加一些分布不均的点到地图上,地图上添加三个点标记，作为参照
            this.markers.forEach(function (marker) {
                new AMap.Marker({
                    map: that.map,
                    icon: marker.icon,
                    position: [marker.position[0], marker.position[1]],
                    offset: new AMap.Pixel(-13, -30)
                });
            });

        },
        startAnimation() {
            this.marker.moveAlong(this.lineArr, 200);
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
        //搜索框智能加载
        querySearch(queryString, cb) {
            var placelist = this.placelist;
            var ret = queryString ? placelist.filter(this.createFilter(queryString)) : placelist;
            cb(ret);
        },
        createFilter(queryString) {
            return (event) => {
                return (event.address.includes(queryString) == true);
            };
        },
        getInput(item) {
            for (let key in this.placelist) {
                //判断地址表的address是否有匹配上的
                if (this.placelist[key].address == item.address) {
                    NavigateStore.startId = key;
                }
            }
        },
        //课程导航提交
        submitclassNav(index, row) {
            for (let key in this.placelist) {
                //判断地址表的address是否有匹配上的
                if (this.placelist[key].address == row.place.slice(0, 2)) {
                    NavigateStore.endId = key;
                }
            }
            // 起始点id， 终止点id
            const getPath = async () => {
                const flag = await NavigateStore.GetNavigatePath(NavigateStore.startId, NavigateStore.endId)
                if (flag) {
                    // 从pinia传来的数据
                    this.lineArr = NavigateStore.rdata.node_list
                    console.log(this.lineArr);
                    this.firstArr = this.lineArr[0];
                    this.initMap();
                    // 将执行导航的信息添加到日志中
                    const log = {
                        "create_time": TimeStore.getTime(),
                        "log_content": "进行导航---从" + this.placelist[NavigateStore.startId].address + "到" + this.placelist[NavigateStore.endId].address,
                    }
                    console.log(log)
                    LogStore.AddLog(log)
                } else {
                    alert("您还未输入起始位置");
                    console.log('未输入起始位置')
                }
            }
            getPath()
        },
        //课外活动导航提交
        submitOutEventNav(index, row) {
            for (let key in this.placelist) {
                if (this.placelist[key].address == row.place) {
                    NavigateStore.endId = key;
                }
            }
            // 起始点id， 终止点id
            const getPath = async () => {
                const flag = await NavigateStore.GetNavigatePath(NavigateStore.startId, NavigateStore.endId)
                if (flag) {
                    // 从pinia传来的数据
                    this.lineArr = NavigateStore.rdata.node_list
                    this.firstArr = this.lineArr[0];
                    this.initMap();
                    // 将执行导航的信息添加到日志中
                    const log = {
                        "create_time": TimeStore.getTime(),
                        "log_content": "进行导航---从" + this.placelist[NavigateStore.startId].address + "到" + this.placelist[NavigateStore.endId].address,
                    }
                    console.log(log)
                    LogStore.AddLog(log)
                } else {
                    alert("您还未输入起始位置");
                    console.log('未输入起始位置')
                }
            }
            getPath()
        },
        // 临时事务导航提交
        submitTempEventNav() {
            let passArray = [];
            for (let i = 0; i < this.multipleSelection.length; i++) {
                for (let key in this.placelist) {
                    //判断地址表的address是否有匹配上的
                    if (this.placelist[key].address == this.multipleSelection[i].place) {
                        passArray.push(parseInt(key));
                    }
                }
            }
            let jso = {};
            for (let i = 0; i < passArray.length; i++) {
                jso[i] = passArray[i]
            }
            let passIds = JSON.stringify(jso);
            const getPath = async () => {
                const flag = await NavigateStore.GetTSPPath(NavigateStore.startId, passIds)
                if (flag) {
                    // 从pinia传来的数据
                    console.log(NavigateStore.tspPath)
                    this.lineArr = NavigateStore.tspPath.node_list;
                    this.firstArr = this.lineArr[0];
                    let passlist = NavigateStore.tspPath.pass_list;
                    this.markers = [];
                    for (let i = 0; i < passlist.length; i++) {
                        this.markers.push({
                            icon: 'https://i.328888.xyz/2023/04/09/icaVW8.png',
                            offset: new AMap.Pixel(-13, -30),
                            position: passlist[i],
                        })
                    }
                    this.initMap();
                    this.nmarker();
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '寻路成功',
                        type: 'success'
                    });
                    // 将执行导航的信息添加到日志中
                    const log = {
                        "create_time": TimeStore.getTime(),
                        "log_content": "进行了多目标路径导航---" + "起始地点:" + this.placelist[NavigateStore.startId].address + "经过地点:" + this.multipleSelection[0].place + "等" + this.multipleSelection.length + "个地点",
                    }
                    console.log(log)
                    LogStore.AddLog(log)
                    

                } else {
                    console.log('error')
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '寻路失败',
                        type: 'error'
                    });
                }
            }
            getPath();

        },
        //获取多选的事务
        handleSelectionChange(val) {
            this.multipleSelection = val;
        }
    }
}
</script>
<style>
.cell {
    margin-left: 10px;
}

.list-group {
    color: #adb5bd;
    margin-top: 10px;
}

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

.el-form-item__label {
    padding: 0 66px 0 0;
}

.el-form {
    padding: 15px;
}

.btn:hover {
    box-shadow: 0 12px 16px 0 rgba(0, 0, 0, 0.24), 0 17px 50px 0 rgba(0, 0, 0, 0.19);
}

.map {
    margin-top: 20px;
}
</style>
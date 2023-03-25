<template>
    <div v-loading="loading" element-loading-text="正在为您检索您可能想去的地方" element-loading-spinner="el-icon-loading"
        element-loading-background="rgba(0, 0, 0, 0.8)">
        <div class="map" style="float:left;">

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
        <div class="search" style="float:right;">
            <el-autocomplete v-model="inputValue" :fetch-suggestions="querySearch" @select="handleSelect"
                placeholder="请输入您当前的位置" :trigger-on-focus="false" size="mini" class="el-auto">
            </el-autocomplete>
            <br>
            <el-radio v-model="radio" label="1">课程导航</el-radio>
            <el-radio v-model="radio" label="2">课外活动导航</el-radio>
            <el-radio v-model="radio" label="3">临时事务导航</el-radio>

            <el-table :data="curcourseList" style="width: 100%">
                <el-table-column label="节次" width="180">
                    <template slot-scope="scope">
                        <i class="el-icon-time"></i>
                        <span style="margin-left: 10px">{{ scope.row.time }}</span>
                    </template>
                </el-table-column>
                <el-table-column label="课程名称" width="180">
                    <template slot-scope="scope">
                        <span style="margin-left: 10px">{{ scope.row.name }}</span>
                    </template>
                </el-table-column>
                <el-table-column label="操作">
                    <template slot-scope="scope">
                        <el-button size="mini" @click="handleEdit(scope.$index, scope.row)">去上这节课</el-button>
                    </template>
                </el-table-column>
            </el-table>
            <!-- <el-form-item label="终点">
                    <el-input v-model="keyWord2" style="width:250px;"></el-input>
                </el-form-item>
                <ul class="list-group">
                    <li v-for="(p, index) of filplacelist2" :key="index" @click="chooseaddress2($event)"
                        style="font-size:13px;">
                        {{ p.address }}
                    </li>
                </ul> -->
            <!-- <el-form-item>
                    <el-button type="primary" @click="onSubmit">查询路线</el-button>
                </el-form-item> -->
            <!-- </el-form> -->
        </div>
        <el-button @click="testTsp">测试tsp问题</el-button>
    </div>
</template>
<script>
import { useNavigateStore } from '@/pinia/modules/navigate'
import { CourseStore } from '@/store/course';
import { useTimeStore } from '@/store/time';

// 绘制线路需要的坐标
// var lineArr = [[116.35530714718364, 39.96393072423919], [116.35542348293764, 39.964436412717816], [116.35600217544192, 39.9646045260412]];
export default {
    data() {
        return {
            loading: false,
            firstArr: [116.35530714718364, 39.96393072423919],// 中心点/初始坐标
            // lineArr: [[116.35530714718364, 39.96393072423919], [116.35542348293764, 39.964436412717816], [116.35600217544192, 39.9646045260412]], // 路径上的点
            lineArr: [], // 路径上的点
            keyWord1: '', //用户输入的关键字(起点)
            keyWord2: '', //用户输入的关键字(终点)
            filplacelist1: [], //模糊匹配后的列表（起点
            filplacelist2: [], //模糊匹配后的列表（终点
            dialogVisible: true,
            radio: '1',//单选框的选择
            multipleSelection: [],//复选表格的所有选择
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
                { id: 37, address: "教四(东门)", value: "教四(东门)" },
                { id: 38, address: "教四(南门)", value: "教四(南门)" },
                { id: 39, address: "西大门", value: "西大门" },
                { id: 40, address: "校训石", value: "校训石" },
                { id: 41, address: "主席像", value: "主席像" },
                { id: 42, address: "主楼", value: "主楼" },
                { id: 43, address: "教三(北门)", value: "教三(北门)" },
                { id: 44, address: "教三(西门)", value: "教三(西门)" },
                { id: 45, address: "教三(东门)", value: "教三(东门)" },
                { id: 46, address: "校车发车点", value: "校车发车点" },
                { id: 47, address: "校医院", value: "校医院" },
                { id: 48, address: "中门邮局", value: "中门邮局" },
                { id: 49, address: "教二(西门)", value: "教二(西门)" },
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
                { id: 62, address: "教一楼(西门)", value: "教一楼(西门)" },
                { id: 63, address: "教一楼(南门)", value: "教一楼(南门)" },
                { id: 64, address: "教一楼(东门)", value: "教一楼(东门)" },
                { id: 65, address: "体育馆", value: "体育馆" },
                { id: 66, address: "游泳馆", value: "游泳馆" },
                { id: 67, address: "科学会堂", value: "科学会堂", },
            ],
            mytime: useTimeStore(),
            curcourseList: [],
            courseList: [],
            inputValue: "",
            useNavigateStore: new useNavigateStore(),
            restaurants: [
                { "value": "三全鲜食（北新泾店）", "address": "长宁区新渔路144号" },
                { "value": "Hot honey 首尔炸鸡（仙霞路）", "address": "上海市长宁区淞虹路661号" },
                { "value": "新旺角茶餐厅", "address": "上海市普陀区真北路988号创邑金沙谷6号楼113" },
                { "value": "泷千家(天山西路店)", "address": "天山西路438号" },
                { "value": "胖仙女纸杯蛋糕（上海凌空店）", "address": "上海市长宁区金钟路968号1幢18号楼一层商铺18-101" },
                { "value": "贡茶", "address": "上海市长宁区金钟路633号" },
                { "value": "豪大大香鸡排超级奶爸", "address": "上海市嘉定区曹安公路曹安路1685号" },
                { "value": "茶芝兰（奶茶，手抓饼）", "address": "上海市普陀区同普路1435号" },
                { "value": "十二泷町", "address": "上海市北翟路1444弄81号B幢-107" },
                { "value": "星移浓缩咖啡", "address": "上海市嘉定区新郁路817号" },
                { "value": "阿姨奶茶/豪大大", "address": "嘉定区曹安路1611号" },
                { "value": "新麦甜四季甜品炸鸡", "address": "嘉定区曹安公路2383弄55号" },
                { "value": "Monica摩托主题咖啡店", "address": "嘉定区江桥镇曹安公路2409号1F，2383弄62号1F" },
                { "value": "浮生若茶（凌空soho店）", "address": "上海长宁区金钟路968号9号楼地下一层" },
                { "value": "NONO JUICE  鲜榨果汁", "address": "上海市长宁区天山西路119号" },
                { "value": "CoCo都可(北新泾店）", "address": "上海市长宁区仙霞西路" },
                { "value": "快乐柠檬（神州智慧店）", "address": "上海市长宁区天山西路567号1层R117号店铺" },
                { "value": "Merci Paul cafe", "address": "上海市普陀区光复西路丹巴路28弄6号楼819" },
                { "value": "猫山王（西郊百联店）", "address": "上海市长宁区仙霞西路88号第一层G05-F01-1-306" },
                { "value": "枪会山", "address": "上海市普陀区棕榈路" },
                { "value": "纵食", "address": "元丰天山花园(东门) 双流路267号" },
                { "value": "钱记", "address": "上海市长宁区天山西路" },
                { "value": "壹杯加", "address": "上海市长宁区通协路" },
                { "value": "唦哇嘀咖", "address": "上海市长宁区新泾镇金钟路999号2幢（B幢）第01层第1-02A单元" },
                { "value": "爱茜茜里(西郊百联)", "address": "长宁区仙霞西路88号1305室" },
                { "value": "爱茜茜里(近铁广场)", "address": "上海市普陀区真北路818号近铁城市广场北区地下二楼N-B2-O2-C商铺" },
                { "value": "鲜果榨汁（金沙江路和美广店）", "address": "普陀区金沙江路2239号金沙和美广场B1-10-6" },
                { "value": "开心丽果（缤谷店）", "address": "上海市长宁区威宁路天山路341号" },
                { "value": "超级鸡车（丰庄路店）", "address": "上海市嘉定区丰庄路240号" },
                { "value": "妙生活果园（北新泾店）", "address": "长宁区新渔路144号" },
                { "value": "香宜度麻辣香锅", "address": "长宁区淞虹路148号" },
                { "value": "凡仔汉堡（老真北路店）", "address": "上海市普陀区老真北路160号" },
                { "value": "港式小铺", "address": "上海市长宁区金钟路968号15楼15-105室" },
                { "value": "蜀香源麻辣香锅（剑河路店）", "address": "剑河路443-1" },
                { "value": "北京饺子馆", "address": "长宁区北新泾街道天山西路490-1号" },
                { "value": "饭典*新简餐（凌空SOHO店）", "address": "上海市长宁区金钟路968号9号楼地下一层9-83室" },
                { "value": "焦耳·川式快餐（金钟路店）", "address": "上海市金钟路633号地下一层甲部" },
                { "value": "动力鸡车", "address": "长宁区仙霞西路299弄3号101B" },
                { "value": "浏阳蒸菜", "address": "天山西路430号" },
                { "value": "四海游龙（天山西路店）", "address": "上海市长宁区天山西路" },
                { "value": "樱花食堂（凌空店）", "address": "上海市长宁区金钟路968号15楼15-105室" },
                { "value": "壹分米客家传统调制米粉(天山店)", "address": "天山西路428号" },
                { "value": "福荣祥烧腊（平溪路店）", "address": "上海市长宁区协和路福泉路255弄57-73号" },
                { "value": "速记黄焖鸡米饭", "address": "上海市长宁区北新泾街道金钟路180号1层01号摊位" },
                { "value": "红辣椒麻辣烫", "address": "上海市长宁区天山西路492号" },
                { "value": "(小杨生煎)西郊百联餐厅", "address": "长宁区仙霞西路88号百联2楼" },
                { "value": "阳阳麻辣烫", "address": "天山西路389号" },
                { "value": "南拳妈妈龙虾盖浇饭", "address": "普陀区金沙江路1699号鑫乐惠美食广场A13" }
            ],
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

        },

    },
    created() { },
    beforeMount() {
        const getTable = async () => {
            const fg = await CourseStore.GetCourseTable();
            if (fg) {
                this.courseList = CourseStore.courseList;
                //根据当前周，查找在本周的课程
                this.courseList = this.courseList.filter((item) => {
                    return item.week_schedule.indexOf(this.mytime.week) != -1;
                });
                //查找本天的课程，然后将他们按照顺序排列。
                for (let i = 0; i < this.courseList.length; i++) {
                    for (let j = 0; j < this.courseList[i].section_list.length; j++) {
                        if (this.courseList[i].section_list[j] / 9 < this.mytime.day) {
                            this.curcourseList.push({
                                name: this.courseList[i].course_name,
                                time: this.courseList[i].section_list[j] % 9,
                            })
                        }
                        if (this.courseList[i].section_list[j] / 9 == 1) {
                            this.curcourseList.push({
                                name: this.courseList[i].course_name,
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
                console.log('获取用户课程失败')
            }
        }
        getTable();
    },
    mounted() {
        setTimeout(() => {
            this.initMap() // 异步加载（否则报错initMap is not defined）
            // this.initroad()
        }, 1000)
    },
    methods: {
        handleClose(done) {
            this.$confirm('确认关闭？')
                .then(_ => {
                    done();
                })
                .catch(_ => { });
        },
        toggleSelection(rows) {
            if (rows) {
                rows.forEach(row => {
                    this.$refs.multipleTable.toggleRowSelection(row);
                });
            } else {
                this.$refs.multipleTable.clearSelection();
            }
        },
        handleSelectionChange(val) {
            this.multipleSelection = val;
        },
        //给后端发送起始点的id和终止点的id
        getNavigatePath: async function (startId, endId) {
            return await this.useNavigateStore.GetNavigatePath(startId, endId)
        },

        onSubmit() {
            var startId = -1, endId = -1;
            for (let key in this.placelist) {
                //判断地址表的address是否有匹配上的
                if (this.placelist[key].address == this.keyWord1) {
                    startId = key;
                }
            }
            for (let key in this.placelist) {
                if (this.placelist[key].address == this.keyWord2) {
                    endId = key;
                }
            }
            //最后改成给后端发送startid和endid
            // alert("起始点id:"+startId+"， 起始点地址："+this.placelist[startId].address+" ,终止点id："+endId+"， 终止点地址："+this.placelist[endId].address);

            // 起始点id， 终止点id
            const getPath = async () => {
                const flag = await this.getNavigatePath(startId, endId)
                if (flag) {
                    // 从pinia传来的数据
                    console.log(this.useNavigateStore.rdata)
                    this.lineArr = this.useNavigateStore.rdata.node_list
                    this.firstArr = this.lineArr[0];
                    this.initMap();
                } else {
                    console.log('error')
                }
            }
            getPath()

        },
        testTsp() {
            let startId = 10
            let passIds = '{ "0": 1, "1": 4, "2": 13, "3": 30, "4": 41, "5": 42, "6": 43, "7": 44, "8": 3, "9": 17, "10": 18, "11": 19, "12": 50, "13": 59 }'

            const getPath = async () => {
                const flag = await this.useNavigateStore.GetTSPPath(startId, passIds)
                if (flag) {
                    // 从pinia传来的数据
                    console.log(this.useNavigateStore.tspPath)
                    this.lineArr = this.useNavigateStore.tspPath.node_list
                    this.firstArr = this.lineArr[0];
                    this.initMap();
                } else {
                    console.log('error')
                }
            }
            getPath()

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
                // autoRotation: true, // 自动旋转
                // angle: -90 // 图片旋转角度
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
        handleSelect(item) {
            console.log(item);
        },
    }
}
</script>
<style>
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
    background-color: #e7f5ff;
    padding: 15px;

    box-shadow: 0px 5px 5px #c8c8c8;
}

.btn:hover {
    box-shadow: 0 12px 16px 0 rgba(0, 0, 0, 0.24), 0 17px 50px 0 rgba(0, 0, 0, 0.19);
}

.map {
    margin-top: 20px;
}

.input-card {
    margin-top: 50px;
}

.list-group {
    margin-top: -18px;
    margin-left: 41px;
    height: 100px;
    overflow-y: scroll;
    z-index: 9999;
}

.list-group::-webkit-scrollbar {
    width: 0;
}

.list-group li {
    list-style: none;
    padding: 1px;
    cursor: pointer;
}

.list-group li:hover {
    color: #1971c2;
}
</style>
<template>
    <div class="container">
        <div class="schedule-container">
            <h2>日程安排表</h2>
            <div class="table-container">

                <el-table :data="filteredData" :default-sort="{ prop: 'start_day', order: 'ascending' }" :filters="filters"
                    filter-method="filterTable" max-height="350px" height="350px" :fixed="true">
                    <el-table-column prop="start_day" label="日期" :formatter="formatDay" sortable
                        :sort-method="sortDay"></el-table-column>
                    <el-table-column prop="start_time" label="时间" sortable></el-table-column>
                    <el-table-column prop="activity_name" label="活动名称"></el-table-column>
                    <el-table-column prop="tag" label="标签" :filters="tagFilters" :filter-method="filterTag"
                        :formatter="formatTag"></el-table-column>
                    <el-table-column align="right">
                        <template slot="header" slot-scope="scope">
                            <el-input v-model="searchText" placeholder="请输入名称关键词"></el-input>
                        </template>
                        <template slot-scope="scope">
                            <el-button type="primary" size="mini" @click="handleClick(scope.row)">查看详情</el-button>
                            <el-button type="danger" size="mini"
                                @click="handleDelete(scope.$index, scope.row)">删除</el-button>
                        </template>
                    </el-table-column>
                </el-table>
            </div>
        </div>
        <eventDialog :selected="selected" :dialogDetailVisible="dialogDetailVisible"></eventDialog>
        <!-- 按键部分 -->
        <div class="button-container">
            <!-- 添加活动按钮和弹窗 -->
            <el-button class="OperationButton" type="primary" icon="el-icon-circle-plus-outline"
                @click="dialogAddVisible = true">添加活动</el-button>
            <el-dialog title="添加活动" :visible="dialogAddVisible">
                <el-radio-group v-model="addEventData.activityType">
                    <el-radio-button label="group">集体活动</el-radio-button>
                    <el-radio-button label="personal">个人活动</el-radio-button>
                    <el-radio-button label="temp">临时活动</el-radio-button>
                </el-radio-group>

                <el-form v-if="addEventData.activityType === 'group'">
                    <el-form-item label="活动名称">
                        <el-input v-model="addEventData.name"></el-input>
                    </el-form-item>
                    <el-form-item label="请选择周次">
                        <el-form-item>
                            <el-select v-model="addEventData.week">
                                <el-option v-for="n in 16" :key="n" :label="`第 ${n} 周`" :value="n"></el-option>
                            </el-select>
                        </el-form-item>
                    </el-form-item>
                    <el-form-item label="请选择星期">
                        <el-select v-model="addEventData.weekday">
                            <el-option label="星期一" value="1"></el-option>
                            <el-option label="星期二" value="2"></el-option>
                            <el-option label="星期三" value="3"></el-option>
                            <el-option label="星期四" value="4"></el-option>
                            <el-option label="星期五" value="5"></el-option>
                            <el-option label="星期六" value="6"></el-option>
                            <el-option label="星期日" value="7"></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="请选择时间">
                        <el-select v-model="addEventData.time">
                            <el-option v-for="time in times" :key="time" :label="time" :value="time"></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="活动频次">
                        <el-radio-group v-model="addEventData.frequency">
                            <el-radio label="1">单次</el-radio>
                            <el-radio label="2">每天一次</el-radio>
                            <el-radio label="3">每周一次</el-radio>
                        </el-radio-group>
                    </el-form-item>
                    <el-form-item label="活动地点">
                        <el-autocomplete v-model="addEventData.location" :fetch-suggestions="querySearch" placeholder="请输入您当前的位置"
                        @select="getInput" :trigger-on-focus="false" class="el-auto">
                    </el-autocomplete>
                    </el-form-item>
                    <el-form-item label="是否添加提醒">
                        <el-radio-group v-model="addEventData.ismention">
                            <el-radio label="true">是</el-radio>
                            <el-radio label="false">否</el-radio>
                        </el-radio-group>
                    </el-form-item>
                    <el-form-item label="提前提醒时间" v-if="addEventData.ismention === 'true'">
                        <el-select v-model="addEventData.advance_mention_time" placeholder="请选择">
                            <el-option label="5分钟" value="5"></el-option>
                            <el-option label="15分钟" value="15"></el-option>
                            <el-option label="30分钟" value="30"></el-option>
                            <el-option label="45分钟" value="45"></el-option>
                            <el-option label="60分钟" value="60"></el-option>
                        </el-select>
                    </el-form-item>
                </el-form>
                <el-form v-else-if="addEventData.activityType === 'personal'">
                    <el-form-item label="活动名称">
                        <el-input v-model="addEventData.name"></el-input>
                    </el-form-item>
                    <el-form-item label="请选择周次">
                        <el-form-item>
                            <el-select v-model="addEventData.week">
                                <el-option v-for="n in 16" :key="n" :label="`第 ${n} 周`" :value="n"></el-option>
                            </el-select>
                        </el-form-item>
                    </el-form-item>
                    <el-form-item label="请选择星期">
                        <el-select v-model="addEventData.weekday">
                            <el-option label="星期一" value="1"></el-option>
                            <el-option label="星期二" value="2"></el-option>
                            <el-option label="星期三" value="3"></el-option>
                            <el-option label="星期四" value="4"></el-option>
                            <el-option label="星期五" value="5"></el-option>
                            <el-option label="星期六" value="6"></el-option>
                            <el-option label="星期日" value="7"></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="请选择时间">
                        <el-select v-model="addEventData.time">
                            <el-option v-for="time in times" :key="time" :label="time" :value="time"></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="活动频次">
                        <el-radio-group v-model="addEventData.frequency">
                            <el-radio label="1">单次</el-radio>
                            <el-radio label="2">每天一次</el-radio>
                            <el-radio label="3">每周一次</el-radio>
                        </el-radio-group>
                    </el-form-item>
                    <el-form-item label="活动地点">
                       <el-autocomplete v-model="addEventData.location" :fetch-suggestions="querySearch" placeholder="请输入您当前的位置"
                    @select="getInput" :trigger-on-focus="false" class="el-auto">
                </el-autocomplete>
                    </el-form-item>
                    <el-form-item label="是否添加提醒">
                        <el-radio-group v-model="addEventData.ismention">
                            <el-radio label="true">是</el-radio>
                            <el-radio label="false">否</el-radio>
                        </el-radio-group>
                    </el-form-item>
                     <el-form-item label="提前提醒时间" v-if="addEventData.ismention === 'true'">
                            <el-select v-model="addEventData.advance_mention_time" placeholder="请选择">
                                <el-option label="5分钟" value="5"></el-option>
                                <el-option label="15分钟" value="15"></el-option>
                                <el-option label="30分钟" value="30"></el-option>
                                <el-option label="45分钟" value="45"></el-option>
                                <el-option label="60分钟" value="60"></el-option>
                            </el-select>
                        </el-form-item>
                </el-form>
                <el-form v-else-if="addEventData.activityType === 'temp'">
                    <el-form-item label="活动名称">
                        <el-input v-model="addEventData.name"></el-input>
                    </el-form-item>
                    <el-form-item label="活动时间">
                        <el-time-picker placeholder="选择时间" v-model="addEventData.time"
                            style="width: 100%;"></el-time-picker>
                    </el-form-item>
                    <el-form-item label="周次">
                        <el-select v-model="addEventData.week" placeholder="请选择" @change="getWeekDays">
                            <el-option v-for="week in weeks" :key="week" :label="`第 ${week} 周`" :value="week"></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="星期">
                        <el-select v-model="addEventData.weekday" placeholder="请选择">
                            <el-option v-for="day in days" :key="day" :label="`星期${day}`" :value="day"></el-option>
                        </el-select>
                    </el-form-item>
                    <el-form-item label="活动地点">
                        <el-autocomplete v-model="addEventData.location" :fetch-suggestions="querySearch" placeholder="请输入您当前的位置"
                        @select="getInput" :trigger-on-focus="false" class="el-auto">
                    </el-autocomplete>
                    </el-form-item>
                    <el-form-item label="是否添加提醒">
                        <el-radio-group v-model="addEventData.ismention">
                            <el-radio label="true">是</el-radio>
                            <el-radio label="false">否</el-radio>
                        </el-radio-group>
                    </el-form-item>
                     <el-form-item label="提前提醒时间" v-if="addEventData.ismention === 'true'">
                            <el-select v-model="addEventData.advance_mention_time" placeholder="请选择">
                                <el-option label="5分钟" value="5"></el-option>
                                <el-option label="15分钟" value="15"></el-option>
                                <el-option label="30分钟" value="30"></el-option>
                                <el-option label="45分钟" value="45"></el-option>
                                <el-option label="60分钟" value="60"></el-option>
                            </el-select>
                        </el-form-item>
                </el-form>
                <div slot="footer">
                    <el-button @click.native="dialogAddVisible = false">取消</el-button>
                    <el-button type="primary" @click.native="submitAddForm">确定</el-button>
                </div>
            </el-dialog>
            <!-- 闹钟提醒 -->
            <el-button class="OperationButton" type="primary" icon="el-icon-alarm-clock">闹钟提醒</el-button>
        </div>
    </div>
</template>

<script>
import eventDialog from '../components/eventDialog.vue'  //引入弹窗组件
import { EventStore } from '@/store/event';
const sectionMap = [{
    value: 1,
    label: '第一节'
}, {
    value: 2,
    label: '第二节'
}, {
    value: 3,
    label: '第三节'
}, {
    value: 4,
    label: '第四节'
}, {
    value: 5,
    label: '第五节'
}, {
    value: 6,
    label: '第六节'
}, {
    value: 7,
    label: '第七节'
}, {
    value: 8,
    label: '第八节'
}, {
    value: 9,
    label: '第九节'
},
];
import { NavigateStore } from "@/store/navigate"
import { LogStore } from '@/store/log';
import { TimeStore } from '@/store/time';

export default {
    data() {
        return {
            frequency: '',
            selected: {},//选择查看详情的活动对象
            dialogDetailVisible: false,
            dialogAddVisible: false, //添加活动的弹窗是否可见
            submit: {},
            eventList: [
            ],
            tagFilters: [
                { text: '个人活动', value: 0 },
                { text: '集体活动', value: 1 },
                { text: '临时活动', value: 2 }
            ],
            filters: {
                tag: []
            },
            addEventData: {
                activityType: 'group',
            },
            deleteEventData: {},
            radio: '',
            value1: [new Date(2016, 9, 10, 8, 40), new Date(2016, 9, 10, 9, 40)],
            searchText: '',
            weeks: Array.from({ length: 15 }, (_, i) => i + 1),
            days: ['一', '二', '三', '四', '五', '六', '日'],
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
            weekday: '',
            hour: '',
            times: ['08:00', '09:00', '10:00', '11:00', '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00']
        }
    },
    created() {
        const getActivityTable = async () => {
            const fg = await EventStore.GetEventTable()
            if (fg) {
                this.eventList = EventStore.eventList
                console.log(this.eventList);
            } else {
                console.log('error')
            }
        }
        getActivityTable()
    },
    methods: {
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
        sortDay(a, b, order) {
            const dayMap = {
                '周一': 1,
                '周二': 2,
                '周三': 3,
                '周四': 4,
                '周五': 5,
                '周六': 6,
                '周日': 7
            }
            const aDay = dayMap[this.formatDay(a)]
            const bDay = dayMap[this.formatDay(b)]
            return order === 'ascending' ? bDay - aDay : aDay - bDay
        },
        formatDay(row, column) {
            const dayMap = {
                '1': '周一',
                '2': '周二',
                '3': '周三',
                '4': '周四',
                '5': '周五',
                '6': '周六',
                '7': '周日'
            }
            return dayMap[row.start_day]
        },
        formatTag(row, column) {
            const tagMap = {
                '0': '个人活动',
                '1': '集体活动',
                '2': '临时活动'
            }
            return tagMap[row.tag]
        },
        filterTable(value, row, column) {
            const property = column['property']
            return row[property] === value
        },
        filterTag(value, row) {
            return row.tag === value
        },
        //删除活动按钮,row即为活动对象
        handleDelete(index, row) {
            console.log(row);
            this.deleteEventData = row.activity_id;
            const deleteEvent = async (data) => {
                const fg = await EventStore.DeleteEventInfo(data);
                if (fg) {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '删除活动成功',
                        type: 'success'
                    });
                    // 创建一个日志对象
                    const log = {
                        "create_time":  TimeStore.getTime(),
                        "content": "删除活动成功, 活动名为：" + row.activity_name,
                    }
                    console.log(log)
                    LogStore.AddLog(log)  
                    this.deleteEventData = {}
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '删除活动失败',
                        type: 'error'
                    });
                    this.deleteEventData = {}
                }
            }

            deleteEvent(this.deleteEventData)
        },
        // 查看详情按钮
        handleClick(row) {
            this.selected = row;
            this.dialogDetailVisible = true;
        },
        submitAddForm() {
            this.dialogAddVisible = false;
            this.submit = {};
            this.submit.activity_name = this.addEventData.name;
            this.submit.start_time = "第" + this.addEventData.week + "周-星期" + this.addEventData.weekday + "-" + this.addEventData.time;
            this.submit.frequency = this.addEventData.frequency;
            this.submit.is_mention = this.addEventData.ismention;
            this.submit.advance_mention_time = this.addEventData.advance_mention_time;
            this.submit.location = this.addEventData.location;

            if (this.addEventData.activityType == 'personal') {
                this.submit.type = 0;
                this.submit.tag = "个人活动";
            }
            else if (this.addEventData.activityType == 'group') {
                this.submit.type = 0;
                this.submit.tag = "集体活动";
            }
            else if (this.addEventData.activityType == 'temp') {
                this.submit.type = 1;
            }
            const addEvent = async (data) => {
                const fg = await EventStore.AddEventInfo(data);
                if (fg) {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '添加活动成功',
                        type: 'success'
                    });
                    // 创建一个日志对象
                    const log = {
                        "create_time":  TimeStore.getTime(),
                        "content": "添加活动成功, 活动名为：" + data.activity_name,
                    }
                    console.log(log)
                    LogStore.AddLog(log)  
                    
                } else {
                    console.log('error')
                }
            }
            addEvent(this.submit)
        },
    },
    computed: {
        filteredData() {
            if (!this.searchText) {
                return this.eventList
            }
            const searchText = this.searchText.toLowerCase()
            return this.eventList.filter(item => item.activity_name.toLowerCase().includes(searchText))
        }

    },
    components: {
        eventDialog,
    }
}
</script>
<style>
.table-container {
    height: 350px;
    overflow-y: auto;
}

.schedule-container {
    margin-top: 40px;
}

.el-input__inner {
    height: 39px;
    line-height: 30px;
}

h2 {
    margin: 15px;
    margin-left: 200px;
    font-size: 20px;
}

.button-container {
    margin-top: 20px;
    margin-left: 130px;
}

.OperationButton {
    margin: 20px
}

.weekinput {
    width: 30px;
    margin: 0 5px;
}
</style>
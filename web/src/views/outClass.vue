<template>
    <div class="container">
        <div class="schedule-container">
            <h2 style="text-align: center; padding-right: 105px;">日程安排表</h2>
            <div class="table-container">
                <el-table :data="filteredData" :default-sort="{ prop: 'start_day', order: 'ascending' }" :filters="filters"
                    filter-method="filterTable" max-height="370px" height="370px" :fixed="true">

                    <el-table-column label="开始日期" :formatter="formatDay" sortable :sort-method="sortDay" align="center">
                        <template slot-scope="scope">
                            <span>{{ scope.row.date }}</span>
                        </template>
                    </el-table-column>


                    <el-table-column label="时间" sortable align="center">
                        <template slot-scope="scope">
                            <i class="el-icon-time"></i> &nbsp
                            <span>{{ scope.row.type == 0 ? scope.row.time : scope.row.start_time }}</span>
                        </template>
                    </el-table-column>


                    <el-table-column label="活动名称及频率" align="center">
                        <template slot-scope="scope">
                            <span>{{ scope.row.activity_name }} {{ scope.row.frequency }}</span>
                        </template>
                    </el-table-column>

                    <el-table-column label="标签" :filters="tagFilters" :filter-method="filterTag" :formatter="formatTag"
                        align="center">
                        <template slot-scope="scope">
                            <el-tag
                                :type="scope.row.tag === '集体活动' ? 'primary' : (scope.row.tag === '临时活动' ? 'success' : 'warning')">{{
                                    scope.row.tag }}</el-tag>
                        </template>
                    </el-table-column>


                    <el-table-column align="right">
                        <template slot="header" slot-scope="scope">
                            <el-input v-model="searchText" placeholder="请输入名称关键词" prefix-icon="el-icon-search"></el-input>
                        </template>
                        <template slot-scope="scope">
                            <el-button type="primary" size="mini" @click="handleClick(scope.row)">查看详情</el-button>
                            <el-button type="danger" size="mini"
                                @click="handleDelete(scope.$index, scope.row)">删除活动</el-button>
                        </template>
                    </el-table-column>
                </el-table>
            </div>
        </div>

        <eventDialog :selected="selected" :dialogDetailVisible="dialogDetailVisible"></eventDialog>

        <div style="display: flex;justify-content:space-between">
            <div class="activity-proportion">
                <h4 style="text-align: center; color:#409eff; margin-left: 160px;">活动占比</h4>
                <el-progress :percentage="singleActivityRate" color="#e6a23c" style="margin: 0px"></el-progress>
                <el-progress :percentage="groupActivityRate" color="#409eff" style="margin: 3px 0px"></el-progress>
                <el-progress :percentage="tempRate" color="#67c23a" style="margin: 0px"></el-progress>
            </div>
            <!-- 按键部分 -->
            <div class="button-container">
                <!-- 查询活动 -->
                <el-button type="success" @click="showDialog" icon="el-icon-search" style="margin-right: 5px;">查询活动</el-button>
                <el-dialog title="查询活动" :visible.sync="showDialogVisible" >
                    <el-form :model="queryForm" label-width="140px">
                        <el-row>
                            <el-col :span="12">
                                <el-form-item label="起始时间">
                                    <el-select v-model="queryForm.startTime.week" placeholder="请选择周数">
                                        <el-option v-for="week in weekOptions" :key="week" :label="week"
                                            :value="week"></el-option>
                                    </el-select>
                                    <el-select v-model="queryForm.endTime.day" placeholder="请选择星期">
                        <el-option v-for="(day, index) in dayOptions" :key="index" :label="day.label" :value="day.value"></el-option>
                    </el-select>
                                    <el-select v-model="queryForm.startTime.time" placeholder="请选择时间">
                                        <el-option v-for="time in timeOptions" :key="time" :label="time"
                                            :value="time"></el-option>
                                    </el-select>
                                </el-form-item>
                            </el-col>
                            <el-col :span="12">
                                <el-form-item label="终止时间">
                                    <el-select v-model="queryForm.endTime.week" placeholder="请选择周数">
                                        <el-option v-for="week in weekOptions" :key="week" :label="week"
                                            :value="week"></el-option>
                                    </el-select>
                                <el-select v-model="queryForm.startTime.day" placeholder="请选择星期">
                        <el-option v-for="(day, index) in dayOptions" :key="index" :label="day.label" :value="day.value"></el-option>
                    </el-select>
                                    <el-select v-model="queryForm.endTime.time" placeholder="请选择时间">
                                        <el-option v-for="time in timeOptions" :key="time" :label="time"
                                            :value="time"></el-option>
                                    </el-select>
                                </el-form-item>
                            </el-col>
                        </el-row>
                    </el-form>
                    <div slot="footer">
                        <el-button @click="showDialogVisible = false">取消</el-button>
                        <el-button type="primary" @click="query">确认</el-button>
                    </div>
                </el-dialog>
                <!-- 添加活动按钮和弹窗 -->
                <el-button class="OperationButton" type="primary" icon="el-icon-circle-plus-outline"
                    @click="dialogAddVisible = true">添加活动</el-button>
                <el-dialog :visible.sync="dialogAddVisible" width="60%">
                    <div slot="title" style="text-align: center; font-size: 16px; font-weight: bold;">添加活动</div>

                    <el-radio-group v-model="addEventData.activityType">
                        <el-radio-button label="group">集体活动</el-radio-button>
                        <el-radio-button label="personal">个人活动</el-radio-button>
                        <el-radio-button label="temp">临时活动</el-radio-button>
                    </el-radio-group>

                    <el-form v-if="addEventData.activityType === 'group'">
                        <el-form-item label="活动名称">
                            
                            <el-input v-model="addEventData.name"></el-input>
                        </el-form-item>

                        <el-form-item label="活动成员">
                        
                            <el-transfer v-model="selectedActivityMembers" :data="studentsInfo" :titles="['学生列表', '活动成员列表']"></el-transfer>
                            
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
                                <el-radio label="0">单次</el-radio>
                                <el-radio label="1">每天一次</el-radio>
                                <el-radio label="2">每周一次</el-radio>
                            </el-radio-group>
                        </el-form-item>
                        <el-form-item label="活动地点">
                            <el-autocomplete v-model="addEventData.location" :fetch-suggestions="querySearch"
                                placeholder="请输入活动地点" @select="getInput" :trigger-on-focus="false" class="el-auto">
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
                                <el-radio label="0">单次</el-radio>
                                <el-radio label="1">每天一次</el-radio>
                                <el-radio label="2">每周一次</el-radio>
                            </el-radio-group>
                        </el-form-item>
                        <el-form-item label="活动地点">
                            <el-autocomplete v-model="addEventData.location" :fetch-suggestions="querySearch"
                                placeholder="请输入活动地点" @select="getInput" :trigger-on-focus="false" class="el-auto">
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
                                style="width: 100%;" :picker-options="{ format: 'HH:mm' }" value-format="HH:mm"></el-time-picker>
                        </el-form-item>
                        <el-form-item label="周次">
                            <el-select v-model="addEventData.week" placeholder="请选择" @change="getWeekDays">
                                <el-option v-for="week in weeks" :key="week" :label="`第 ${week} 周`" :value="week"></el-option>
                            </el-select>
                        </el-form-item>
                        <el-form-item label="星期">
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
                        <el-form-item label="活动地点">
                            <el-autocomplete v-model="addEventData.location" :fetch-suggestions="querySearch"
                                placeholder="请输入活动地点" @select="getInput" :trigger-on-focus="false" class="el-auto">
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
                        <el-button @click.native="dialogAddVisible = false;selectedActivityMembers=[]">取消</el-button>
                        <el-button type="primary" @click.native="submitAddForm">确定</el-button>
                    </div>
                </el-dialog>
            </div>
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
import { UserStore } from '@/store/user';

export default {
    data() {
        return {
            selectedActivityMembers: [],
            studentsInfo: [],
            isStudentsListVisible: false,
            singleActivityRate: 0,
            groupActivityRate: 0,
            tempActivityRate: 0,
            frequency: '',
            selected: {},//选择查看详情的活动对象
            dialogDetailVisible: false,
            dialogAddVisible: false, //添加活动的弹窗是否可见
            submit: {},
            eventList: [
            ],
            tagFilters: [
                { text: '个人活动', value: '个人活动' },
                { text: '集体活动', value: '集体活动' },
                { text: '临时活动', value: '临时活动' }
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
            tempRate:1,
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
            times: ['08:00', '09:00', '10:00', '11:00', '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00'],
            showDialogVisible: false,
            queryForm: {
                startTime: {
                    week: '',
                    day: '',
                    time: ''
                },
                endTime: {
                    week: '',
                    day: '',
                    time: ''
                }
            },
            weekOptions: Array.from({ length: 16 }, (v, i) => `第${i + 1}周`),
            dayOptions: [
                { label: '周一', value: '周一' },
                { label: '周二', value: '周二' },
                { label: '周三', value: '周三' },
                { label: '周四', value: '周四' },
                { label: '周五', value: '周五' },
                { label: '周六', value: '周六' },
                { label: '周日', value: '周七' }
            ],
            timeOptions: ['8:00', '9:00', '10:00', '11:00', '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00'],
            searchEventlist: [],
        }
    },
    created() {

        //获取活动
        const getActivityTable = async () => {
            const fg = await EventStore.GetEventTable()
            if (fg) {
                this.eventList = EventStore.eventList
                // 将this.eventList.start_time的后五位去掉
                let num0 = 0
                let num1 = 0
                let num2 = 0
                for (let i = 0; i < this.eventList.length; i++) {
                    console.log(this.eventList[i].tag)
                    if (this.eventList[i].tag == "个人活动") {
                        num0++
                    } else if (this.eventList[i].tag == "集体活动"){
                        num1++
                    } else {
                        num2++
                    }
                    this.eventList[i].date = this.eventList[i].start_time.slice(0, -6)
                }
                if (this.eventList.length != 0) {
                    this.singleActivityRate = (num0 / this.eventList.length) * 100
                    this.groupActivityRate = (num1 / this.eventList.length) * 100
                    this.tempRate = (num2 / this.eventList.length) * 100
                    // 三者保留一位小数
                    this.singleActivityRate = this.singleActivityRate.toFixed(1)
                    this.groupActivityRate = this.groupActivityRate.toFixed(1)
                    this.tempRate = this.tempRate.toFixed(1)
                }
               
                for (let i = 0; i < this.eventList.length; i++) {
                    if (this.eventList[i].frequency == 0) {
                        this.eventList[i].frequency = '(单次)'
                    } else {
                        if (this.eventList[i].frequency == 1) {
                            this.eventList[i].frequency = '(每天)'
                        } else {
                            this.eventList[i].frequency = '(每周)'
                        }
                    }
                }
                for (let i = 0; i < this.eventList.length; i++) {
                    // 将this.eventList.start_time的后五位取出
                    this.eventList[i].start_time = this.eventList[i].start_time.slice(-5)
                    // 提取this.eventList.start_time的前两个字符
                    let x = Number(this.eventList[i].start_time.slice(0, 2))
                    console.log(x)
                    x = x + 1;
                    if (x < 10) {
                        x = '0' + x
                    }
                    // 将this.eventList.start_time的前两个字符替换为x
                    this.eventList[i].end_time = x + this.eventList[i].start_time.slice(-3)


                    this.eventList[i].time = this.eventList[i].start_time + ' ~ ' + this.eventList[i].end_time
                }

            } else {
                console.log('error')
            }
        }
        getActivityTable()

         //获取学生信息
        const getStudentsInfo = async () => {
            const fg = await UserStore.GetAllStuInfo()
            if (fg) {
                this.studentsInfo = UserStore.allStuInfo
                for (let i = 0; i < this.studentsInfo.length; i++) {
                    this.studentsInfo[i].key = this.studentsInfo[i].user_id
                    if (this.studentsInfo[i].user_id == window.localStorage.getItem('userId')) {
                        this.studentsInfo[i].label = '我'
                        this.studentsInfo[i].disabled = true
                    } else {
                        this.studentsInfo[i].disabled = false
                        this.studentsInfo[i].label = this.studentsInfo[i].username
                    }
                }
                console.log(this.studentsInfo)
            } else {
                console.log('error')
            }
        }
        getStudentsInfo()
    },

    methods: {
        showDialog() {
            this.showDialogVisible = true
        },
         // 发送查询请求
        query() {
            let start_time = this.queryForm.startTime.week + "-" + this.queryForm.startTime.day + "-" + this.queryForm.startTime.time;
            let end_time = this.queryForm.endTime.week + "-" + this.queryForm.endTime.day + "-" + this.queryForm.endTime.time;
            const searchEvent = async (start_time, end_time) => {
                const fg = await EventStore.SearchEventInfo(start_time, end_time);
                if (fg) {
                    this.searchEventlist = CourseStore.searchlist;

                } else {
                    console.log('error')
                }
            }
            searchEvent(start_time, end_time)
            this.showDialogVisible = false
        },

        // 搜索框智能加载
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
            console.log(row.tag)
            return tagMap[row.tag]
        },
        filterTable(value, row, column) {
            const property = column['property']
            return row[property] === value
        },
        filterTag(value, row) {
            return row.tag == value
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
                        "create_time": TimeStore.getTime(),
                        "content": "删除活动成功, 活动名为：" + row.activity_name,
                    }
                    location.reload();
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
            this.submit.frequency = Number(this.addEventData.frequency);
            this.submit.is_mention = this.addEventData.ismention = 'true' ? true : false;
            this.submit.advance_mention_time = Number(this.addEventData.advance_mention_time);
            this.submit.location = this.addEventData.location;
            

            if (this.addEventData.activityType == 'personal') {
                this.submit.type = 0;
                this.submit.tag = "个人活动";
            }
            else if (this.addEventData.activityType == 'group') {
                this.submit.members = this.selectedActivityMembers
                console.log(this.submit.members)
                this.submit.type = 0;
                this.submit.tag = "集体活动";
            }
            else if (this.addEventData.activityType == 'temp') {
                this.submit.type = 1;
                this.submit.tag = "临时活动";
            }
            console.log(this.submit)
            const addEvent = async (data) => {
                const fg = await EventStore.AddEventInfo(data);
                if (fg) {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '添加活动成功',
                        type: 'success'
                    });
                    this.submit = {};
                    this.addEventData = {};
                    // 创建一个日志对象
                    const log = {
                        "create_time": TimeStore.getTime(),
                        "content": "添加活动成功, 活动名为：" + data.activity_name,
                    }
                    console.log(log)
                    LogStore.AddLog(log)

                    // 刷新页面
                    location.reload();
                } else {
                    console.log('error')
                }
            }
            addEvent(this.submit)
            this.selectedActivityMembers = []



        },
    },
    computed: {
        transFormToDate() {
            return (startTime) => {
                // 去掉startTime的后6位
                const date = startTime.substring(0, startTime.length - 6)
                return date
            }
        },
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
.el-transfer__buttons {
    padding: 0 10px;
}
.activity-proportion {
    width: 400px;
   
}
.table-container {
    height: 370px;
    overflow-y: auto;
}

.schedule-container {
    margin-top: 20px;
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
    float: left;
    margin-top: 4px;
}

.OperationButton {
    margin-top: 20px;
    margin-right: 1px;
}

.weekinput {
    width: 30px;
    margin: 0 5px;
}
</style>
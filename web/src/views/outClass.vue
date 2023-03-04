<template>
    <div class="container">
        <div class="schedule-container">
            <h2>日程安排表</h2>
            <div>
                <el-table
                    :data="eventlist.filter(data => !search || data.event.includes(search) || data.week.includes(search) || data.time.includes(search))"
                    style="width: 100%" max-height="350">
                    <el-table-column label="日期" prop="week" fixed sortable
                        :sort-method="(a, b) => a.week.localeCompare(b.week)">
                    </el-table-column>
                    <el-table-column label="时间" prop="time" fixed sortable
                        :sort-method="(a, b) => a.time.localeCompare(b.time)">
                    </el-table-column>
                    <el-table-column label="事件" prop="event" fixed>
                    </el-table-column>
                    <el-table-column prop="tag" label="标签" width="100"
                        :filters="[{ text: '集体活动', value: '集体活动' }, { text: '个人活动', value: '个人活动' }]"
                        :filter-method="filterTag" filter-placement="bottom-end">
                        <template slot-scope="scope">
                            <el-tag :type="scope.row.tag === '集体活动' ? 'primary' : 'success'" disable-transitions>{{
                                scope.row.tag }}</el-tag>
                        </template>
                    </el-table-column>
                    <el-table-column align="right">
                        <template slot="header" slot-scope="scope">
                            <el-input v-model="search" size="mini" placeholder="输入关键词(时间/事件)搜索" />
                        </template>
                        <template slot-scope="scope">
                            <el-button type="primary" size="mini" @click="handleClick(scope.row)">查看详情</el-button>
                            <!-- <el-button type="info" size="mini" @click="handleEdit(scope.$index, scope.row)">编辑</el-button> -->
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
            <el-dialog title="添加活动" :visible.sync="dialogAddVisible">
                <el-form :model="form">
                    <el-form-item label="活动名称" :label-width="formLabelWidth">
                        <el-input v-model="form.event" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="活动地点" :label-width="formLabelWidth">
                        <el-input v-model="form.address" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="开始周次" :label-width="formLabelWidth">
                        <el-input v-model="form.startweek" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="结束周次" :label-width="formLabelWidth">
                        <el-input v-model="form.endweek" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="活动时间" :label-width="formLabelWidth">
                        <el-input v-model="form.time" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="活动类型" :label-width="formLabelWidth">
                        <el-radio v-model="radio" label="1">个人活动</el-radio>
                        <el-radio v-model="radio" label="2">集体活动</el-radio>
                    </el-form-item>
                </el-form>
                <div slot="footer" class="dialog-footer">
                    <el-button @click="dialogAddVisible = false">取 消</el-button>
                    <el-button type="primary" @click="dialogAddVisible = false">确 定</el-button>
                </div>
            </el-dialog>
            <!-- 闹钟提醒 -->
            <el-button class="OperationButton" type="primary" icon="el-icon-alarm-clock">闹钟提醒</el-button>
        </div>
    </div>
</template>

<script>
import eventDialog from '../components/eventDialog.vue'  //引入弹窗组件
export default {
    data() {
        return {
            selected: {},//选择查看详情的活动对象
            dialogDetailVisible: false,
            dialogAddVisible: false, //添加活动的弹窗是否可见
            //增加活动时表单中填的数据
            form: {
                week: '',
                time: '',
                event: '',
                tag: '',
                address: '',
                startweek: '',
                endweek: '',
            },
            radio: '',
            search: '', //用于搜索过滤的对象
            formLabelWidth: '120px',
            value1: [new Date(2016, 9, 10, 8, 40), new Date(2016, 9, 10, 9, 40)],
            eventlist: [{
                week: '周3',
                time: '15:00',
                event: '小组作业',
                tag: "集体活动",
                address: "教室",
            }, {
                week: '周4',
                time: '14:00',
                event: '取快递',
                address: "教室",
                tag: "个人活动",
            }, {
                week: '周5',
                time: '14:00',
                event: '计算机网络考试',
                address: "教室",
                tag: "集体活动",
            }, {
                week: '周5',
                time: '17:00',
                event: '出门游玩',
                address: "教室",
                tag: "集体活动",

            }, {
                week: '周1',
                time: '14:00',
                event: '锻炼',
                address: "教室",
                tag: "个人活动",

            }, {
                week: '周2',
                time: '12:00',
                event: '去超市',
                address: "教室",
                tag: "个人活动",

            }, {
                week: '周4',
                time: '07:00',
                event: '创新创业',
                address: "教室",
                tag: "个人活动",
            }, {
                week: '周3',
                time: '11:00',
                event: '开班会',
                address: "教室",
                tag: "集体活动",

            },]
        }
    },
    methods: {
        //删除活动按钮,row即为活动对象
        handleDelete(index, row) {
            console.log(row);
        },
        //对标签进行筛选
        filterTag(value, row) {
            return row.tag === value;
        },
        // 查看详情按钮
        handleClick(row) {
            this.selected = row;
            this.dialogDetailVisible = true;
        },
    },
    components: {
        eventDialog,
    }
}
</script>
<style>
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
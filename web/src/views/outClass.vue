<template>
    <el-tabs type="border-card">
        <el-tab-pane>
            <span slot="label"><i class="el-icon-date"></i> 个人活动</span>
            <div class="container">
                <div class="schedule-container">
                    <h2>日程安排表</h2>
                    <div>
                        <el-table :data="eventlist.filter(data => !search || data.name.toLowerCase().includes(search.toLowerCase()))"
                            style="width: 100%"  max-height="350">
                            <el-table-column label="日期" prop="date"  fixed  sortable :sort-method="(a, b) => a.date.localeCompare(b.date)"  >
                            </el-table-column>
                            <el-table-column label="时间" prop="time" fixed sortable :sort-method="(a, b) => a.time.localeCompare(b.time)">
                            </el-table-column>
                            <el-table-column label="事件" prop="event" fixed >
                            </el-table-column>
                            <el-table-column align="right">
                                <template slot="header" slot-scope="scope">
                                    <el-input v-model="search" size="mini" placeholder="输入关键字搜索" />
                                </template>
                                <template slot-scope="scope">
                                    <el-button size="mini" @click="handleEdit(scope.$index, scope.row)">编辑</el-button>
                                    <el-button size="mini" type="danger"
                                        @click="handleDelete(scope.$index, scope.row)">删除</el-button>
                                </template>
                            </el-table-column>
                        </el-table>
                    </div>
                </div>
                <!-- 按键部分 -->
                <div class="button-container">
                    <!-- 添加活动 -->
                    <el-button class="OperationButton" type="primary" icon="el-icon-circle-plus-outline"
                        @click="dialogFormVisible = true">添加活动</el-button>
                    <el-dialog title="添加活动" :visible.sync="dialogFormVisible">
                        <el-form :model="form">
                            <el-form-item label="活动名称" :label-width="formLabelWidth">
                                <el-input v-model="form.name" autocomplete="off"></el-input>
                            </el-form-item>
                            <el-form-item label="活动地点" :label-width="formLabelWidth">
                                <el-input v-model="form.address" autocomplete="off"></el-input>
                            </el-form-item>
                            <el-form-item label="活动起止时间" :label-width="formLabelWidth">
                                <el-time-picker is-range v-model="value1" range-separator="至" start-placeholder="开始时间"
                                    end-placeholder="结束时间" placeholder="选择时间范围">
                                </el-time-picker>
                            </el-form-item>
                            <el-form-item label="活动类型" :label-width="formLabelWidth">
                                <el-radio v-model="radio" label="1">个人活动</el-radio>
                                <el-radio v-model="radio" label="2">集体活动</el-radio>
                            </el-form-item>
                        </el-form>
                        <div slot="footer" class="dialog-footer">
                            <el-button @click="dialogFormVisible = false">取 消</el-button>
                            <el-button type="primary" @click="dialogFormVisible = false">确 定</el-button>
                        </div>
                    </el-dialog>
                    <el-button class="OperationButton" type="primary" icon="el-icon-search">活动查询</el-button>
                    <el-button class="OperationButton" type="primary" icon="el-icon-alarm-clock">闹钟提醒</el-button>
                </div>
            </div>
        </el-tab-pane>
        <el-tab-pane>
            <span slot="label"><i class="el-icon-s-flag"></i> 集体活动</span>
            集体活动
        </el-tab-pane>
    </el-tabs>
</template>

<script>
export default {
    data() {
        return {
            dialogFormVisible: false,
            form: {
                name: '',
                address: '',
            },
            search: '',
            formLabelWidth: '120px',
            value1: [new Date(2016, 9, 10, 8, 40), new Date(2016, 9, 10, 9, 40)],
            eventlist: [{
                date: '周三',
                time: '15:00',
                event: '班会'
            }, {
                date: '周四',
                time: '14:00',
                event: '取快递'
            }, {
                date: '周五',
                time: '14:00',
                event: '计算机网络考试'
            }, {
                date: '周五',
                time: '17:00',
                event: '出门游玩'
            }, {
                date: '周一',
                time: '14:00',
                event: '游玩'
            }, {
                date: '周二',
                time: '12:00',
                event: '去超市'
            }, {
                date: '周四',
                time: '7:00',
                event: '买菜'
            }, {
                date: '周三',
                time: '11:00',
                event: '开班会'
            },]
        }
    },
    methods: {
        formatter(row, column) {
            return row.name;
        },
        handleEdit(index, row) {
            console.log(index, row);
        },
        handleDelete(index, row) {
            console.log(index, row);
        },
    }
}
</script>
<style>
h2 {
    margin: 15px;
    margin-left: 200px;
    font-size: 20px;
}

.button-container {
    margin-top: 10px;
}

.OperationButton {
    margin: 20px
}
</style>
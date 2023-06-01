import { service } from '@/utils/request'

// 课外活动接口
export const EventTable = () => {
    return service({
        url: '/activity/info',
        method: 'get'
    })
}
// 添加活动
export const AddEvent = (data) => {
    return service({
        url: '/activity/add',
        method: 'post',
        data: data
    })
}

// 删除活动
export const DeleteEvent= (data) => {
    return service({
        url: '/activity/delete?activity_id=' + data,
        method: 'delete',
    })
}
// 查询课程
export const SearchEvent = (fromTime, endTime) => {
    const url = '/activity/query?from_time=' + fromTime + 'end_time=' + endTime;
    return service({
        url: url,
        method: 'get',
    })
}